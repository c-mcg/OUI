#include "oui/comp/OUI_Component.h"
#include <sstream>
#include "oui/OUI_Window.h"
#include "OUI.h"
#include "OUI_Constants.h"
#include "util/OUI_StringUtil.h"

#ifdef _DEBUG

#include "oui/debug/OUI_Inspector.h"

#endif

oui::Style* oui::Component::defaultStyle = NULL;

oui::Style* oui::Component::getDefaultComponentStyle() {
	if (Component::defaultStyle == NULL) {
		Style* style = new Style();

		AttributeProfile* profile = new AttributeProfile("Component::defaultStyle['default']");

		profile->setAttribute("cursor", u"default");
		profile->setAttribute("opacity", 255);
		profile->setAttribute("border-style", u"none");
		profile->setAttribute("centered-x", false);
		profile->setAttribute("centered-y", false);
		profile->setAttribute("border-width", 1);
		profile->setAttribute("width-percent", 0);
		profile->setAttribute("height-percent", 0);
		profile->setAttribute("width-offset", 100);
		profile->setAttribute("height-offset", 100);
		profile->setAttribute("x-percent", 0);
		profile->setAttribute("y-percent", 0);
		profile->setAttribute("x-offset", 0);
		profile->setAttribute("y-offset", 0);
		profile->setAttribute("visible", true);
		profile->setAttribute("interactable", true);
		profile->setAttribute("bg-color1-r", 255);
		profile->setAttribute("bg-color1-g", 255);
		profile->setAttribute("bg-color1-b", 255);
		profile->setAttribute("bg-color1-a", 255);
		profile->setAttribute("bg-color2-r", 255);
		profile->setAttribute("bg-color2-g", 255);
		profile->setAttribute("bg-color2-b", 255);
		profile->setAttribute("bg-color2-a", 255);
		profile->setAttribute("border-color-r", 0);
		profile->setAttribute("border-color-g", 0);
		profile->setAttribute("border-color-b", 0);
		profile->setAttribute("border-color-a", 255);
		profile->setAttribute("min-width", 0);
		profile->setAttribute("min-height", 0);
		profile->setAttribute("z-index", 0);
		profile->setAttribute("right-click-options_length", 0);
		profile->setAttribute("permanent", false);

		style->addProfile(u"default", profile);
		style->addProfile(u"hover", new AttributeProfile("Component::defaultStyle['hover']"));

		Component::defaultStyle = style;
	}

	return Component::defaultStyle;
}

oui::Component::~Component() {
	delete style;
	delete definedStyle;
	classes.clear();
	parent = NULL;
	window = NULL;
	delete graphics;
}

oui::Component::Component(std::string tag, std::string name, std::string classes, bool needsProcessing) : needsProcessing(true) {
	
	this->tag = tag;
	this->name = name;
	
	this->window = NULL;
	this->parent = NULL;
	this->graphics = NULL;

	this->classes = std::vector<std::string>();
	std::istringstream iss(classes.c_str());

	std::string className;
	while(iss >> className) {
		if(className == "") {
			continue;
		}
		this->classes.push_back(className);
	}
	
	definedStyle = new Style();
	style = new Style();

	style->getOrCreateProfile(u"default")->componentName = this->name + ":default";
	style->getOrCreateProfile(u"hover")->componentName = this->name + ":hover";

	this->currentProfile = u"default";

}

/* START OF PROCESSING */

int oui::Component::process() {
	return 0;
}
void oui::Component::redraw() {
	graphics->clear();
	if (backgroundColor1.equals(backgroundColor2)) {
		graphics->setColor(backgroundColor1);
		graphics->fillRect(0, 0, getWidth(), getHeight());
	} else {
		graphics->fillVerticalGradient(0, 0, getWidth(), getHeight(), backgroundColor1, backgroundColor2);
	}
	graphicsUpdate = false;
	if (!isContainer()) {
		drawBorder();
	}
}

void oui::Component::drawBorder() {
	if(borderStyle != BORDER_NONE) {
		if(borderStyle == BORDER_SOLID) {
			graphics->setColor(borderColor);
			for(int i = 0; i < borderWidth; i++) {
				graphics->drawRect(i, i, getWidth() - i * 2, getHeight() - i * 2);
			}
		}
	}
}
void oui::Component::addedToContainer(Container* parent) {

	Style* defaultStyle = getDefaultStyle();

	AttributeProfile* defaultProfile = style->getOrCreateProfile(u"default");
	AttributeProfile* hoverProfile = style->getOrCreateProfile(u"hover");
	
	defaultProfile->clearDefaultProfiles();
	defaultProfile->addDefaultProfile(defaultStyle->getOrCreateProfile(u"default"));

	hoverProfile->clearDefaultProfiles();
	hoverProfile->addDefaultProfile(defaultStyle->getOrCreateProfile(u"hover"));

	hoverProfile->addDefaultProfile(defaultProfile);

	Component::setProfile(u"default");//TODO this needs to be here to ensure graphics are initialized but why?
	setProfile(u"default");

	window = (Window*) (parent->isWindow() ? parent : parent->window);
	this->parent = parent;
	//createStyle();
	if (window != NULL) {
		graphics->setSize(getWidth(), getHeight());
		graphics->setRenderer(window->graphics->getRenderer());
	} else {
		
	}
}

/* END OF PROCESSING */


/* START OF EVENTS */

void oui::Component::handleEvent(Event& e) {

	if (e.isMouseEvent()) {
		MouseEvent& mouseEvt = (MouseEvent&) e;

		mouseX = mouseEvt.getX();
		mouseY = mouseEvt.getY();

		auto it = mouseEventHandlers.find(e.type);

		if (it != mouseEventHandlers.end()) {

			auto handlers = it->second;

			for (auto it = handlers.begin(); it != handlers.end(); it++) {
				(*it)(mouseEvt, this);
			}
		}
	} else if (e.isMenuEvent()) {

		MenuEvent& menuEvt = (MenuEvent&) e;

#ifdef _DEBUG
		if (menuEvt.getOption() == u"Inspect") {
			Inspector::showInspector(this, this->window);
		}
#endif

		for (auto it = menuEventHandlers.begin(); it != menuEventHandlers.end(); it++) {
			(*it)(menuEvt, this);
		}

	} else if (e.isScrollEvent()) {

		ScrollEvent& scrollEvt = (ScrollEvent&) e;

		for (auto it = scrollEventHandlers.begin(); it != scrollEventHandlers.end(); it++) {
			(*it)(scrollEvt, this);
		}

	} else if (e.isWindowEvent()) {
		WindowEvent& windowEvt = (WindowEvent&) e;

		auto it = windowEventHandlers.find(e.type);

		if (it != windowEventHandlers.end()) {

			auto handlers = it->second;

			for (auto it2 = handlers.begin(); it2 != handlers.end(); it2++) {
				(*it2)(windowEvt, this);
			}
		}
	} else if (e.isKeyEvent()) {
		KeyEvent& keyEvt = (KeyEvent&) e;

		auto it = keyEventHandlers.find(e.type);

		if (it != keyEventHandlers.end()) {

			auto handlers = it->second;

			for (auto it2 = handlers.begin(); it2 != handlers.end(); it2++) {
				(*it2)(keyEvt, this);
			}
		}
	}

	if (parent != NULL) {
		//if (e.)
		//parent->handleEvent(e);
	}

}

void oui::Component::addEventListener(char type, std::function<void(MouseEvent, Component*)> handler) {
	mouseEventHandlers[type].push_back(handler);
}
void oui::Component::addEventListener(char type, std::function<void(KeyEvent, Component*)> handler) {
	keyEventHandlers[type].push_back(handler);
}
void oui::Component::addEventListener(char type, std::function<void(WindowEvent, Component*)> handler) {
	windowEventHandlers[type].push_back(handler);
}
void oui::Component::addEventListener(char type, std::function<void(ScrollEvent, Component*)> handler) {
	scrollEventHandlers.push_back(handler);
}
void oui::Component::addEventListener(char type, std::function<void(MenuEvent, Component*)> handler) {
	menuEventHandlers.push_back(handler);
}

/* END OF EVENTS */

/* START OF USAGE FUNCTIONS */

void oui::Component::setProfile(std::u16string profile) {
	int w = getWidth(), h = getHeight();
	AttributeProfile* apf = style->getProfile(profile);
	if (apf != NULL) {
		currentProfile = profile;

		//Location
		xPercent = apf->getInt("x-percent");
		yPercent = apf->getInt("y-percent");
		xOffset = apf->getInt("x-offset");
		yOffset = apf->getInt("y-offset");

		//Z-index
		z = apf->getInt("z-index");

		//Interactable
		interactable = apf->getBool("interactable");

		//Size
		minWidth = apf->getInt("min-width");
		minHeight = apf->getInt("min-height");
		widthPercent = apf->getInt("width-percent");
		heightPercent = apf->getInt("height-percent");
		widthOffset = apf->getInt("width-offset");
		heightOffset = apf->getInt("height-offset");

		//Background
		backgroundColor1 = Color(apf->getInt("bg-color1-r"), apf->getInt("bg-color1-g"), apf->getInt("bg-color1-b"), apf->getInt("bg-color1-a"));
		backgroundColor2 = Color(apf->getInt("bg-color2-r"), apf->getInt("bg-color2-g"), apf->getInt("bg-color2-b"), apf->getInt("bg-color2-a"));

		//Centered
		//TODO substitute and stuff
		centeredX = apf->getBool("centered-x");
		centeredY = apf->getBool("centered-y");

		//Border
		borderColor = Color(apf->getInt("border-color-r"), apf->getInt("border-color-g"), apf->getInt("border-color-b"), apf->getInt("border-color-a"));
		borderWidth = apf->getInt("border-width");
		std::u16string borderStyle = apf->getString("border-style");
		if(borderStyle == u"none") {
			this->borderStyle = Component::BORDER_NONE;
		} else if (borderStyle == u"solid") {
			this->borderStyle = Component::BORDER_SOLID;
		}

		//Opacity
		//TODO remove opacity var
		opacity = apf->getInt("opacity");
		if (graphics != NULL) {
			graphics->setAlpha(opacity);
		}

		//Visible
		visible = apf->getBool("visible");

		//Right-click-options
		std::vector<std::u16string> options;
		for (int i = 0; i < apf->getInt("right-click-options_length"); i++) {
			options.push_back(apf->getString("right-click-options_" + std::to_string(i)));
		}
		rightClickOptions = options;

		std::u16string cursor = apf->getString("cursor");
		if (cursor == u"default") {
			this->cursor = CURSOR_DEFAULT;
		} else if (cursor == u"pointer") {
			this->cursor = CURSOR_POINTER;
		} else if (cursor == u"text") {
			this->cursor = CURSOR_TEXT;
		}

	}
	
	if (graphics == NULL) {
		graphics = new Graphics(getWidth(), getHeight());
		graphics->setAlpha(opacity);
		if (window != NULL) {
			graphics->setRenderer(window->graphics->getRenderer());
		}
		flagGraphicsUpdate();
	} else if (graphics->getWidth() != getWidth() || graphics->getHeight() != getHeight()) {
		graphics->setSize(getWidth(), getHeight());
		flagGraphicsUpdate();
	}
	

	width = calculateWidth();
	height = calculateHeight();
}

void oui::Component::parseAttribute(std::string name, std::u16string value, std::u16string profile) {
	//definedStyle->getOrCreateProfile(profile)->parseAttribute(name, value);
	style->getOrCreateProfile(profile)->parseAttribute(name, value);

	flagProfileUpdate();
}

void oui::Component::setAttribute(std::string name, Attribute a, std::u16string profile) {
	//definedStyle->getOrCreateProfile(profile)->setAttribute(name, a);
	style->getOrCreateProfile(profile)->setAttribute(name, a);

	flagProfileUpdate();
}

bool oui::Component::contains(int localX, int localY) {
	return localX >= getX() && localX <= getX() + getWidth() && localY >= getY() && localY <= getY() + getHeight();
}

/* END OF USAGE FUNCTIONS */

/* START CONTAINERS */

bool oui::Component::isWindow() {
	return false;
}
bool oui::Component::isContainer() {
	return false;
}
oui::Window* oui::Component::getWindow() {
	return window;
}
oui::Container* oui::Component::getParent() {
	return parent;
}
bool oui::Component::isChildOf(Component* c) {
	if (parent != NULL) {
		return parent == c ? true : parent->isChildOf(c);
	}
	return false;
}
bool oui::Component::isChildOf(std::string tag) {
	if (parent != NULL) {
		return parent->compareTag(tag) ? true : parent->isChildOf(tag);
	}
	return false;
}

/* END CONTAINERS */


/* START IDENTIFICATION*/

std::string oui::Component::getTag() {
	return tag;
}
bool oui::Component::compareTag(std::string tag) {
	return strcmp(this->tag.c_str(), tag.c_str()) == 0;
}
bool oui::Component::setName(std::string name) {
	if (parent != NULL) {
		if (((Container*) parent)->isDuplicateName(name, this)) {
			std::cout << "Tried to set duplicate name: " << name.c_str() << std::endl;
			return false;
		}
	}
	this->name = name;
	return true;
}
std::string oui::Component::getName() {
	return name;
}
void oui::Component::addClass(std::string _class) {
	classes.push_back(_class);
	createStyle();
}
bool oui::Component::removeClass(std::string _class) {
	for (unsigned int i = 0; i < classes.size(); i++) {
		if (classes.at(i) == _class) {
			classes.erase(classes.begin() + i);
			return true;
		}
	}
	return false;
}
void oui::Component::setClasses(std::string classes) {
	this->classes = std::vector<std::string>();
	std::istringstream iss(classes.c_str());
	std::string currClass;
	while (iss >> currClass) {
		if (currClass == "") {
			continue;
		}
		this->classes.push_back(currClass);
	}
	createStyle();
}
void oui::Component::setClasses(std::vector<std::string> classes) {
	this->classes = std::vector<std::string>(classes);
	createStyle();
}
std::vector<std::string> oui::Component::getClasses() {
	return classes;
}

/* END IDENTIFICATION */



/* START OF SIZE & LOCATION */

bool oui::Component::isDependantOnParent() {//TODO seperate width and height (optimize only updating when needed)
	return widthPercent != 0 || heightPercent != 0 || xPercent != 0 || yPercent != 0;
}
int oui::Component::getWidth() {
	return width;
}
int oui::Component::getHeight() {
	return height;
}

int oui::Component::getX() {
	return calculateX();
}
int oui::Component::getY() {
	return calculateY();
}
int oui::Component::getScreenX() {
	return calculateScreenX();
}
int oui::Component::getScreenY() {
	return calculateScreenY();
}

int oui::Component::calculateWidth() {//TODO cache width
	int width = parent == NULL ? widthOffset : ((float) widthPercent / 100.0) * parent->calculateWidth() + widthOffset;
	return width < minWidth ? minWidth : width;
}
int oui::Component::calculateHeight() {//TODO cache height
	int height = parent == NULL ? heightOffset : ((float) heightPercent / 100.0) * parent->calculateHeight() + heightOffset;
	return height < minHeight ? minHeight : height;
}

int oui::Component::calculateX() {
	//TODO percent based on screen size for Window class
	int offset = (int) (xOffset - (centeredX ? calculateWidth() / 2 : 0));
	return (int) (parent == NULL ? offset : ((float) (xPercent) / 100.0) * parent->calculateWidth() + offset - scrollOffsetX);
}
int oui::Component::calculateY() {
	//TODO percent based on screen size for Window class
	int offset = (int) (yOffset - (centeredY ? calculateHeight() / 2 : 0));
	return parent == NULL ? offset : ((float) (yPercent) / 100.0) * parent->calculateHeight() + offset - scrollOffsetY;
}

int oui::Component::calculateScreenX() {
	return calculateX() + (parent == NULL ? 0 : parent->calculateX());
}
int oui::Component::calculateScreenY() {
	return calculateY() + (parent == NULL ? 0 : parent->calculateY());
}
void oui::Component::setScrollOffsetX(int scrollOffsetX) {
	this->scrollOffsetX = scrollOffsetX;
}
void oui::Component::setScrollOffsetY(int scrollOffsetY) {
	this->scrollOffsetY = scrollOffsetY;
}

/* END OF SIZE & LOCATION */


/* START OF GRAPHICS */

oui::Graphics* oui::Component::getGraphics() {
	return graphics;
}
void oui::Component::flagGraphicsUpdate(bool flagParent) {
	this->graphicsUpdate = true;
	if (flagParent && parent != NULL) {
		parent->flagGraphicsUpdate();
	}
}
void oui::Component::clearGraphicsUpdate() {
	this->graphicsUpdate = false;
}
bool oui::Component::needsGraphicsUpdate() {
	return graphicsUpdate;
}
void oui::Component::flagProfileUpdate() {
	profileUpdate = true;
	flagGraphicsUpdate();
}
bool oui::Component::needsProfileUpdate() {
	return profileUpdate;
}
void oui::Component::clearProfileUpdate() {
	profileUpdate = false;
}

/* END OF GRAPHICS */

/* START OF STATE */


int oui::Component::getMouseX() {
	return mouseX;
}
int oui::Component::getMouseY() {
	return mouseY;
}
void oui::Component::setSelected(bool selected) {
	this->selected = selected;
}
bool oui::Component::isSelected() {
	return selected;
}
void oui::Component::setMouseDown(bool mouseDown) {
	if (this->mouseDown != mouseDown) {
		this->flagGraphicsUpdate();
	}
	this->mouseDown = mouseDown;
	if (mouseDown && parent != NULL) {
		parent->setMouseDown(true);
	}
}
bool oui::Component::isMouseDown() {
	return mouseDown;
}
void oui::Component::setHovered(bool hovered) {
	if (this->hovered != hovered) {
		setProfile(hovered ? u"hover" : u"default");
		this->flagProfileUpdate();
		this->hovered = hovered;
		if (parent != NULL && hovered) {
			parent->setHovered(true);
		}
	}
}
bool oui::Component::isHovered() {
	return hovered;
}

/* END OF STATE */

/* START OF STYLE */

oui::Style* oui::Component::createStyle(StyleSheet* sheet) {

	Style* currentStyle = style;

	if(sheet != NULL || parent != NULL) {

		if (sheet == NULL) {
			sheet = parent->getAllStyleSheets();

			if (sheet == NULL) {
				return NULL;
			}
		}

		//Apply tag styling
		Style* temp = sheet->getByTag(this->tag);//This isn't inline for debugging
		currentStyle->combineStyle(temp, true);


		//Apply class styling
		auto it = this->classes.begin();
		for (; it != this->classes.end(); it++) {
			currentStyle->combineStyle(sheet->getByClass(*it), true);
		}

		//Apply name styling
		currentStyle->combineStyle(sheet->getByName(this->name), true);
	}

	//currentStyle->combineStyle(definedStyle);
	this->style = currentStyle;
	return currentStyle;
}

int oui::Component::getCursor() {
	return cursor;
}

std::vector<std::u16string> oui::Component::getRightClickOptions() {
	AttributeProfile* profile = getCurrentProfile();
	std::vector<std::u16string> options;
	for (int i = 0; i < profile->getInt("right-click-options_length"); i++) {
		options.push_back(profile->getString("right-click-options_" + std::to_string(i)));
	}
	return options;
}

oui::AttributeProfile* oui::Component::getCurrentProfile() {
	//TODO should there be a null check here?
	return style->getProfile(currentProfile);
}

void oui::Component::updateStyle() {
	if (parent != NULL || tag == "window") {
		setProfile(currentProfile);
	}
}

oui::Style* oui::Component::getDefaultStyle() {
	return Component::getDefaultComponentStyle();
}

/* END OF STYLE */