#include "components/container/OUI_Container.h"

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include "window/OUI_window.h"
#include "exception/OUI_ArgumentException.h"

#include "util/OUI_ComponentUtil.h"

oui::Container::~Container() {
    std::vector<Component*>::iterator it;
    for (it = children.begin(); it != children.end(); ) {
        delete *it;
        it = children.erase(it);
    }
    children.clear();
}

oui::Container::Container(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing, EventDispatcher* eventDispatcher, ContainerAttributeManager* attributeManager) :
    Component(tag, name, classes, needsProcessing, eventDispatcher, attributeManager) {
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

void oui::Container::flagGraphicsUpdateAll() {
    //TODO This is slow because flagGraphicsUpdate in component has upwards recursion to parent
    flagGraphicsUpdate();
    for (unsigned int i = 0; i < children.size(); i++) {
        Component* c = children.at(i);
        //if (!c->isDependantOnParent()) continue;

        if (c->isContainer()) {
            static_cast<Container*>(c)->flagGraphicsUpdateAll();
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
    return (int) children.size();
}

void oui::Container::setSelected(bool selected) {
    Component::setSelected(selected);
    if (selected == false) {
        for (unsigned int i = 0; i < children.size(); i++) {
            children.at(i)->setSelected(false);
        }
    }
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
         if (children.at(permanentOffset)->getAttributeManager()->isPermanent()) {
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
    if (!c->getAttributeManager()->isPermanent()) {

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
    ContainerAttributeManager* attributeManager = getAttributeManager();
    for (unsigned int i = 0; i < children.size(); i++) {
        if (children.at(i)->getName() == child->getName()) {
            throw ArgumentException(
                "Container",
                "addChild",
                "Tried to add another child with the same name (name: " + child->getName() + ")",
                "Specify a source component"
            );
        }
    }
    child->addedToContainer(this);
    child->deriveAttributesForComponent(attributeManager->getStyleSheet());
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
        return static_cast<Container*>(c);
    }
    return NULL;
}

oui::Container* oui::Container::getChildCont(int index) {
    Component* c = children.at(index);
    if (c != NULL && c->isContainer()) {
        return static_cast<Container*>(c);
    }
    return NULL;
}

std::vector<oui::Component*> oui::Container::getChildren() {
    return std::vector<oui::Component*>(children);
}

void oui::Container::redraw() {
    Component::redraw();
    redrawChildren();
    drawBorder();
}

void oui::Container::redrawChildren() {
    auto componentsByZIndex = util::orderByZIndex(getChildren());
    for(auto it = componentsByZIndex.begin(); it != componentsByZIndex.end(); it++) {
        std::vector<Component*> comps = it->second;
        for(unsigned int i = 0; i < comps.size(); i++) {
            Component* c = comps.at(i);

            if (!c->getAttributeManager()->isVisible()) {
                continue;
            }

            if (c->needsGraphicsUpdate()) {
                c->refreshProfile();
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
    ContainerAttributeManager* attributeManager = getAttributeManager();
    StyleSheet* sheet = StyleSheet::fromOSAL(osalSheet);
    attributeManager->addStyleSheet(sheet);
}


oui::StyleSheet* oui::Container::getAllStyleSheets() {
    ContainerAttributeManager* attributeManager = getAttributeManager();
    return attributeManager->getAllStyleSheets();
}

oui::ContainerAttributeManager* oui::Container::getAttributeManager() {
    return static_cast<ContainerAttributeManager*>(attributeManager);
}