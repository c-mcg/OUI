#include "oui/comp/OUI_Container.h"
#include "oui/OUI_Window.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

oui::Container::~Container() {
	std::vector<Component*>::iterator it;
	for (it = children.begin(); it != children.end(); ) {
		delete *it;
		it = children.erase(it);
	}
	children.clear();

	if (this->styleSheet != NULL) {
		delete this->styleSheet;
	}
}

oui::Container::Container(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing) : Component(tag, name, classes, needsProcessing) {
	this->styleSheet = NULL;
}

void oui::Container::onProcessableChildAdded(Component* addedChild) {
	this->processableChildren.push_back(addedChild);

	if (parent != NULL) {
		parent->onProcessableChildAdded(addedChild);
	}

}

void oui::Container::onProcessableChildRemoved(Component* removedChild) {
	auto it = std::find(processableChildren.begin(), processableChildren.end(), removedChild);

	if (it != processableChildren.end()) {
		processableChildren.erase(it);
	}

	if (parent != NULL) {
		parent->onProcessableChildRemoved(removedChild);
	}
}

void oui::Container::setProfile(const std::u16string& profile) {
	int w = getWidth(), h = getHeight();
	Component::setProfile(profile);
	if (w != graphics->getWidth() || h != graphics->getHeight()) {
		flagGraphicsUpdateAll();
	}
}

void oui::Container::setStyleSheet(StyleSheet* sheet) {

	if (this->styleSheet != sheet && this->styleSheet != NULL) {
		delete this->styleSheet;
	}

	this->styleSheet = sheet;
}

oui::Style* oui::Container::createStyle(StyleSheet* sheet) {

	if (sheet == NULL) {
		sheet = getAllStyleSheets();

		if (sheet == NULL) {
			return NULL;
		}
	}

	if (sheet != this->styleSheet) {
		StyleSheet* clone = sheet->clone(); //Not inline for debugging
		setStyleSheet(clone);
	}

	for(unsigned int i = 0; i < children.size(); i++) {
		children.at(i)->createStyle(sheet);
	}

	Component::createStyle(sheet);
	return this->style;
}

void oui::Container::flagGraphicsUpdateAll() {
	//TODO This is slow because flagGraphicsUpdate in component has upwards recursion to parent
	flagGraphicsUpdate();
	for (unsigned int i = 0; i < children.size(); i++) {
		Component* c = children.at(i);
		//if (!c->isDependantOnParent()) continue;

		if (c->isContainer()) {
			((Container*) c)->flagGraphicsUpdateAll();
		} else {
			c->flagGraphicsUpdate(false);
		}
	}
}

void oui::Container::addedToContainer(Container* parent) {
	Component::addedToContainer(parent);
	for(unsigned int i = 0; i < children.size(); i++) {
		auto child = children.at(i);
		child->addedToContainer(this);
	}

	for (int i = 0; i < processableChildren.size(); i++) {
		parent->onProcessableChildAdded(processableChildren.at(i));
	}

	flagGraphicsUpdateAll();
}

int oui::Container::getNumChildren() {
	return children.size();
}

void oui::Container::setSelected(bool selected) {
	Component::setSelected(selected);
	if (selected == false) {
		for (unsigned int i = 0; i < children.size(); i++) {
			children.at(i)->setSelected(false);
		}
	}
}

oui::Component* oui::Container::getComponentAt(int x, int y) {
	Component* component = NULL;

	std::unordered_map<int, std::vector<Component*>> zIndexes;
	for (unsigned int i = 0; i < children.size(); i++) {
		Component* child = children.at(i);
		int z = child->getAttribute("z-index", 0).intVal;

        if(child->getCurrentProfile()->getBool("visible") && child->getCurrentProfile()->getBool("interactable")) {
            if(z != 0) {
                if(zIndexes.find(z) == zIndexes.end()) {
                    zIndexes[z] = std::vector<Component*>();
                }
                zIndexes[z].push_back(child);
            }
            if(child->contains(x - (isWindow() ? 0 : this->getX()), y - (isWindow() ? 0 : this->getY()))) {
                component = child->isContainer() ? ((Container*)(child))->getComponentAt(x - (isWindow() ? 0 : getX()), y - (isWindow() ? 0 : getY())) : child;
                break;
            }
        }
	}
	//std::sort(zIndexes.begin(), zIndexes.end(), std::less<int>());
	std::unordered_map<int, std::vector<Component*>>::iterator it;
	for(it = zIndexes.begin(); it != zIndexes.end(); ++it) {
		std::vector<Component*> comps = it->second;
		for(unsigned int i = 0; i < comps.size(); i++) {
			if(comps.at(i)->contains(x - (isWindow() ? 0 : this->getX()), y - (isWindow() ? 0 : this->getY()))) {
				component = comps.at(i)->isContainer() ? static_cast<Container*>(comps.at(i))->getComponentAt(x - (isWindow() ? 0 : getX()), y - (isWindow() ? 0 : getY())) : comps.at(i);
				goto break_outer_loop;
			}
		}
	}
break_outer_loop:

	return component != NULL ? component: this;
}

int oui::Container::getIndexOf(const std::string& name) {
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children.at(i)->getName() == name) {
			return i;
		}
	}
	return -1;
}
int oui::Container::getIndexOf(Component* c) {
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children.at(i) == c) {
			return i;
		}
	}
	return -1;
}

bool oui::Container::isContainer() {
	return true;
}

 void oui::Container::removeAllChildren(bool shouldDelete) {

	 //The offset from the bottom of the contianer due to "permanent" children
	 int permanentOffset = 0;

	 while (children.size() > permanentOffset) {

		 //If a permanent child is at the "bottom" we bump the offset up to leave it there
		 if (children.at(permanentOffset)->getCurrentProfile()->getBool("permanent")) {
			 permanentOffset++;
			 continue;
		 }

		// Remove the child if it's not permanent
		removeChild(permanentOffset, shouldDelete);
	}
}

oui::Component* oui::Container::removeChild(const std::string& name, bool shouldDelete) {
	int index = getIndexOf(name);
	return index == -1 ? NULL : removeChild(index, shouldDelete);
}
oui::Component* oui::Container::removeChild(Component* child, bool shouldDelete) {

	for (int i = 0; i < children.size(); i++) {
		if (children.at(i) == child) {
			return removeChild(i, shouldDelete);
		}
	}

	return NULL;
}
oui::Component* oui::Container::removeChild(int index, bool shouldDelete) {
	if (index < 0 || index >= children.size()) {
		return NULL;
	}

	Component* c = children.at(index);
	if (!c->getCurrentProfile()->getBool("permanent")) {

		if (c->needsProcessing) {
			onProcessableChildRemoved(c);
		}

		children.erase(children.begin() + index);
		flagGraphicsUpdate();

		if (shouldDelete) {
			delete c;
		}

		return NULL;
	}
	
	return c;
}

bool oui::Container::isDuplicateName(const std::string& name, Component* ignore) {
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children.at(i) != ignore && children.at(i)->getName() == name) {
			return true;
		}
	}
	return false;
}

bool oui::Container::addChild(Component* child) {
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children.at(i)->getName() == child->getName()) {
			std::cout << "Tried to add duplicate child: '" << child->getName().c_str() << "'" << std::endl;//TODO error
			return false;
		}
	}
	child->addedToContainer(this);
	child->createStyle(this->styleSheet);
	children.push_back(child);

	if (child->needsProcessing) {
		onProcessableChildAdded(child);
	}

	return true;
}

oui::Component* oui::Container::getChild(const std::string& name) {
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children.at(i)->getName() == name) {
			return getChild(i);
		}
	}
	return NULL;
}

oui::Component* oui::Container::getChild(int index) {
	return children.at(index);
}

oui::Container* oui::Container::getChildCont(const std::string& name) {
	Component* c = getChild(name);
	if (c != NULL && c->isContainer()) {
		return (Container*) c;
	}
	return NULL;
}

oui::Container* oui::Container::getChildCont(int index) {
	Component* c = children.at(index);
	if (c != NULL && c->isContainer()) {
		return (Container*) c;
	}
	return NULL;
}

void oui::Container::redraw() {
	Component::redraw();
	redrawChildren();
	drawBorder();
}

void oui::Container::redrawChildren() {
	std::unordered_map<int, std::vector<Component*>> zIndexes;

	for(unsigned int i = 0; i < children.size(); i++) {
		Component* c = children.at(i);
		if(c->getCurrentProfile()->getBool("visible")) {
			int z = c->getCurrentProfile()->getInt("z-index");
			if(z != 0) {
				if(zIndexes.find(z) == zIndexes.end()) {
					zIndexes[z] = std::vector<Component*>();
				}
				zIndexes[z].push_back(c);
			}
			if (c->needsProfileUpdate()) {
				c->updateStyle();
				c->clearProfileUpdate();
				c->redraw();
			} else if(c->needsGraphicsUpdate()) {
				c->updateStyle();
				c->redraw();
			}
			c->getGraphics()->renderToGraphics(c->getX(), c->getY(), graphics);
		}
	}
	//std::sort(zIndexes.begin(), zIndexes.end(), std::less<int>());

	std::unordered_map<int, std::vector<Component*>>::iterator it;
	for(it = zIndexes.begin(); it != zIndexes.end(); ++it) {
		std::vector<Component*> comps = it->second;
		for(unsigned int i = 0; i < comps.size(); i++) {
			Component* c = comps.at(i);
			if (c->needsGraphicsUpdate()) {
				c->redraw();
			}
			c->getGraphics()->renderToGraphics(c->getX(), c->getY(), graphics);
		}
	}
}

void oui::Container::setHovered(bool hovered) {
	Component::setHovered(hovered);
	if(!hovered) {
		for(unsigned int i = 0; i < children.size(); i++) {
			Component* child = children.at(i);
			child->setHovered(false);
		}
	}
}
void oui::Container::setMouseDown(bool mouseDown) {
	Component::setMouseDown(mouseDown);
	if(!mouseDown) {
		for(unsigned int i = 0; i < children.size(); i++) {
			children.at(i)->setMouseDown(false);
		}
	} 
}

void oui::Container::addOSALStyle(const std::u16string& sheet) {
	OSAL::Sheet sheet2 = OSAL::parseSheet(sheet);
	addOSALStyle(sheet2);
}

void oui::Container::addOSALStyle(OSAL::Sheet osalSheet) {
	StyleSheet* sheet = StyleSheet::fromOSAL(osalSheet);
	addStyleSheet(sheet);
}

void oui::Container::addStyleSheet(StyleSheet* sheet) {
	if(styleSheet == NULL) {
		this->styleSheet = sheet;
	} else {
		this->styleSheet->combineStyleSheet(sheet, true);
	}
	createStyle(this->styleSheet);
	if (this->getCurrentProfile()->getBool("visible")) {
		updateStyle();
	}
}

oui::StyleSheet* oui::Container::getAllStyleSheets() {

	StyleSheet* sheet = new StyleSheet();

	//Add style sheet from parent
	if(parent != NULL) {

		StyleSheet* parentSheet = parent->getAllStyleSheets();

		if (parentSheet != NULL) {
			sheet->combineStyleSheet(parentSheet);
		}

	}

	//Make sure something exists before we add this styleSheet
	//This makes it so we only need to check for empty sheet later, as opposed to looking for equality to styleSheet
	if (!sheet->isEmpty()) {

		//Add this style sheet
		if (this->styleSheet != NULL && !this->styleSheet->isEmpty()) {
			sheet->combineStyleSheet(this->styleSheet, false);
		}

	}

	//Delete and replace current style sheet
	//Above check lets us assume if it's not empty, then it isn't equal to `this->styleSheet`
	if (!sheet->isEmpty()) {

		//Sets the current style sheet, deletes the old one if it exists
		setStyleSheet(sheet);

		return sheet;
	}

	//We didn't replace the styleSheet so we'll delete this
	delete sheet;
	sheet = NULL;

	//Return the current styleSheet
	return styleSheet;
}
