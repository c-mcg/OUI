#include "components/OUI_Component.h"
#include <sstream>
#include "OUI_Window.h"
#include "OUI.h"
#include "OUI_Constants.h"
#include "util/OUI_StringUtil.h"

#ifdef _DEBUG

//#include "debug/OUI_Inspector.h"

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

oui::Component::Component(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing, EventDispatcher* eventDispatcher) : 
    tag{tag}, name{name}, needsProcessing{needsProcessing},
    window{NULL}, graphics{NULL}, parent{NULL},
    x{0}, y{0}, z{0}, screenX{0}, screenY{0}, xPercent{0}, xOffset{0}, yPercent{0}, yOffset{0}, scrollOffsetX{0}, scrollOffsetY{0},
    width{0}, height{0}, minWidth{0}, minHeight{0}, widthPercent{0}, widthOffset{0}, heightPercent{0}, heightOffset{0},
    visible{true}, profileUpdate{false}, graphicsUpdate{true}, currentProfileName{u"default"},
    hovered{false}, selected{false}, mouseDown{false}, interactable{false},
    cursor{CURSOR_DEFAULT}, mouseX{0}, mouseY{0},
	opacity{255}, centeredX{false}, centeredY{false}, borderWidth{0}, borderStyle{BORDER_NONE},
    eventDispatcher{eventDispatcher}
{
    eventDispatcher->setTarget(this);

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

    this->currentProfile = style->getProfile(u"default");
}

void oui::Component::addEventListener(std::string type, EventHandler handler) {
    eventDispatcher->addEventListener(type, handler);
}

void oui::Component::addSystemEventListener(std::string type, EventHandler handler) {
    eventDispatcher->addSystemEventListener(type, handler);
}

oui::Attribute oui::Component::getAttribute(const std::string& name, Attribute defaultVal) {
    Attribute* attr = getCurrentProfile()->getAttribute(name);
    if (attr == NULL) {
        return defaultVal;
    }
    return *attr;
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

oui::EventDispatcher* oui::Component::getEventDispatcher() {
    return eventDispatcher;
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

    window = (Window*) (parent->isWindow() ? parent : parent->window);
    this->parent = parent;
    createStyle();
    if (window != NULL) {
        graphics = window->createGraphics(getWidth(), getHeight());
    }

    if (graphics != NULL) {
        Component::setProfile(u"default");//TODO this needs to be here to ensure graphics are initialized but why?
        setProfile(u"default");
    }

    flagProfileUpdate();
}

/* END OF PROCESSING */

/* START OF USAGE FUNCTIONS */

void oui::Component::setProfile(const std::u16string& profile) {
    AttributeProfile* apf = style->getProfile(profile);
    if (apf != NULL) {
        currentProfileName = profile;
        currentProfile = apf;

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
    
    if (graphics->getWidth() != getWidth() || graphics->getHeight() != getHeight()) {
        graphics->setSize(getWidth(), getHeight());
        flagGraphicsUpdate();
    }

    width = calculateWidth();
    height = calculateHeight();
}

void oui::Component::parseAttribute(const std::string& name, const std::u16string& value, const std::u16string& profile) {
    //definedStyle->getOrCreateProfile(profile)->parseAttribute(name, value);
    style->getOrCreateProfile(profile)->parseAttribute(name, value);

    flagProfileUpdate();
}

void oui::Component::setAttribute(const std::string& name, Attribute a, const std::u16string& profile) {
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
bool oui::Component::isChildOf(const std::string& tag) {
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
bool oui::Component::compareTag(const std::string& tag) {
    return this->tag.compare(tag) == 0;
}
bool oui::Component::setName(const std::string& name) {
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
bool oui::Component::removeClass(const std::string& _class) {
    for (unsigned int i = 0; i < classes.size(); i++) {
        if (classes.at(i) == _class) {
            classes.erase(classes.begin() + i);
            return true;
        }
    }
    return false;
}
void oui::Component::setClasses(const std::string& classes) {
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
    x = calculateX();
    return x;
}
int oui::Component::getY() {
    y = calculateY();
    return y;
}
int oui::Component::getScreenX() {
    return calculateScreenX();
}
int oui::Component::getScreenY() {
    return calculateScreenY();
}

int oui::Component::calculateWidth() {//TODO cache width
    auto width = parent == NULL ? widthOffset : ((float) widthPercent / 100.0) * parent->calculateWidth() + widthOffset;
    return (int) (width < minWidth ? minWidth : width);
}
int oui::Component::calculateHeight() {//TODO cache height
    auto height = parent == NULL ? heightOffset : ((float) heightPercent / 100.0) * parent->calculateHeight() + heightOffset;
    return (int) (height < minHeight ? minHeight : height);
}

int oui::Component::calculateX() {
    //TODO percent based on screen size for Window class
    auto offset = (int) (xOffset - (centeredX ? calculateWidth() / 2 : 0));
    return (int) (parent == NULL ? offset : ((float) (xPercent) / 100.0) * parent->calculateWidth() + offset - scrollOffsetX);
}
int oui::Component::calculateY() {
    //TODO percent based on screen size for Window class
    auto offset = (int) (yOffset - (centeredY ? calculateHeight() / 2 : 0));
    return (int) (parent == NULL ? offset : ((float) (yPercent) / 100.0) * parent->calculateHeight() + offset - scrollOffsetY);
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

    // Style* optimizedStyle = new Style();
    // auto profileNames = currentStyle->getProfileNames();
    // for(auto it = profileNames.begin(); it != profileNames.end(); ++it) {
    //     std::u16string profileName = *it;
    //     AttributeProfile* optimizedProfile = currentStyle->getProfile(profileName)->flatten();
    //     optimizedStyle->addProfile(profileName, optimizedProfile);
    // }

    //currentStyle->combineStyle(definedStyle);
    this->style = currentStyle;
    return this->style;
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
    return currentProfile;
}

void oui::Component::updateStyle() {
    if (parent != NULL || tag == "window") {
        setProfile(currentProfileName);
    }
}

oui::Style* oui::Component::getDefaultStyle() {
    return Component::getDefaultComponentStyle();
}

/* END OF STYLE */