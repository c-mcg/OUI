#include "components/OUI_Component.h"
#include <sstream>
#include "OUI_Window.h"
#include "OUI.h"
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
        profile->setAttribute("bg-color1", Color::WHITE);
        profile->setAttribute("bg-color2", Color::WHITE);
        profile->setAttribute("border-color", Color::BLACK);
        profile->setAttribute("min-width", 0);
        profile->setAttribute("min-height", 0);
        profile->setAttribute("z-index", 0);
        profile->setAttribute("right-click-options", std::vector<Attribute>());
        profile->setAttribute("permanent", false);

        style->addProfile(u"default", profile);
        style->addProfile(u"hover", new AttributeProfile("Component::defaultStyle['hover']"));

        Component::defaultStyle = style;
    }

    return Component::defaultStyle;
}

oui::Component::~Component() {
    delete attributeManager;
    delete eventDispatcher;
    classes.clear();
    parent = NULL;
    window = NULL;
    delete graphics;
}

oui::Component::Component(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing, EventDispatcher* eventDispatcher, ComponentAttributeManager* attributeManager) : 
    tag{tag}, name{name}, needsProcessing{needsProcessing},
    window{NULL}, graphics{NULL}, parent{NULL},
    x{0}, y{0}, screenX{0}, screenY{0}, scrollOffsetX{0}, scrollOffsetY{0},
    width{0}, height{0}, graphicsUpdate{true},
    hovered{false}, selected{false}, mouseDown{false},
    mouseX{0}, mouseY{0}, eventDispatcher{eventDispatcher}, attributeManager{attributeManager}
{
    eventDispatcher->setTarget(this);
    eventDispatcher->addEventListener("mousemove", [this](ComponentEvent* compEvent) {
        MouseEvent* event = static_cast<MouseEvent*>(compEvent);
        this->mouseX = event->localX;
        this->mouseY = event->localY;
    });

    attributeManager->setComponent(this);

    this->classes = std::vector<std::string>();
    std::istringstream iss(classes.c_str());

    std::string className;
    while(iss >> className) {
        if(className == "") {
            continue;
        }
        this->classes.push_back(className);
    }
}

void oui::Component::addEventListener(std::string type, EventHandler handler) {
    eventDispatcher->addEventListener(type, handler);
}

/* START OF PROCESSING */

int oui::Component::process() {
    return 0;
}
void oui::Component::redraw() {
    graphics->clear();
    Color backgroundColor1 = attributeManager->getBackgroundColor1();
    Color backgroundColor2 = attributeManager->getBackgroundColor2();

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
    auto borderStyle = attributeManager->getBorderStyle();
    if(borderStyle == constants::BORDER_NONE) {
        return;
    }

    Color borderColor = attributeManager->getBorderColor();
    int borderWidth = attributeManager->getBorderWidth();
    if(borderStyle == constants::BORDER_SOLID) {
        graphics->setColor(borderColor);
        for(int i = 0; i < borderWidth; i++) {
            graphics->drawRect(i, i, getWidth() - i * 2, getHeight() - i * 2);
        }
    }
}

void oui::Component::addedToContainer(Container* parent) {
    window = static_cast<Window*>(parent->isWindow() ? parent : parent->window);
    this->parent = parent;
    if (window != NULL) {
        graphics = window->createGraphics(getWidth(), getHeight());
    }

    if (graphics != NULL) {
        attributeManager->deriveAttributesForComponent();
        attributeManager->refreshProfile();
    }
}

/* END OF PROCESSING */

/* START OF USAGE FUNCTIONS */

void oui::Component::updateSize() {
    if (graphics == NULL) {
        return;
    }

    if (graphics->getWidth() != getWidth() || graphics->getHeight() != getHeight()) {
        graphics->setSize(getWidth(), getHeight());
        flagGraphicsUpdate();
    }

    width = calculateWidth();
    height = calculateHeight();
}

void oui::Component::parseAttribute(const std::string& name, const std::u16string& value, const std::u16string& profileName) {
    attributeManager->parseAttribute(name, value, profileName);
}

void oui::Component::setAttribute(const std::string& name, Attribute value, const std::u16string& profileName) {
    attributeManager->setAttribute(name, value, profileName);
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
        if (static_cast<Container*> (parent)->isDuplicateName(name, this)) {
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
    attributeManager->deriveAttributesForComponent();
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
    attributeManager->deriveAttributesForComponent();
}
void oui::Component::setClasses(std::vector<std::string> classes) {
    this->classes = std::vector<std::string>(classes);
    attributeManager->deriveAttributesForComponent();
}
std::vector<std::string> oui::Component::getClasses() {
    return classes;
}

/* END IDENTIFICATION */



/* START OF SIZE & LOCATION */

bool oui::Component::isDependantOnParent() {//TODO seperate width and height (optimize only updating when needed)
    int widthPercent = attributeManager->getWidthPercent();
    int heightPercent = attributeManager->getHeightPercent();
    int xPercent = attributeManager->getXPercent();
    int yPercent = attributeManager->getYPercent();
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
    int widthPercent = attributeManager->getWidthPercent();
    int widthOffset = attributeManager->getWidthOffset();
    int minWidth = attributeManager->getMinWidth();
    auto width = parent == NULL ? widthOffset : ((float) widthPercent / 100.0) * parent->calculateWidth() + widthOffset;
    return (int) (width < minWidth ? minWidth : width);
}
int oui::Component::calculateHeight() {//TODO cache height
    int heightPercent = attributeManager->getHeightPercent();
    int heightOffset = attributeManager->getHeightOffset();
    int minHeight = attributeManager->getMinHeight();
    auto height = parent == NULL ? heightOffset : ((float) heightPercent / 100.0) * parent->calculateHeight() + heightOffset;
    return (int) (height < minHeight ? minHeight : height);
}

int oui::Component::calculateX() {
    int xPercent = attributeManager->getXPercent();
    int xOffset = attributeManager->getXOffset();
    int centeredX = attributeManager->getCenteredX();
    //TODO percent based on screen size for Window class
    auto offset = (int) (xOffset - (centeredX ? calculateWidth() / 2 : 0));
    return (int) (parent == NULL ? offset : ((float) (xPercent) / 100.0) * parent->calculateWidth() + offset - scrollOffsetX);
}
int oui::Component::calculateY() {
    int yPercent = attributeManager->getYPercent();
    int yOffset = attributeManager->getYOffset();
    int centeredY = attributeManager->getCenteredY();
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
        attributeManager->setProfile(hovered ? u"hover" : u"default");
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

std::u16string oui::Component::getCursor() {
    return attributeManager->getCursor();
}

std::vector<std::u16string> oui::Component::getRightClickOptions() {
    return attributeManager->getRightClickOptions();
}

void oui::Component::deriveAttributesForComponent(StyleSheet* styleSheet) {
    attributeManager->deriveAttributesForComponent(styleSheet);
}

oui::ComponentAttributeManager* oui::Component::getAttributeManager() {
    return attributeManager;
}

void oui::Component::refreshProfile() {
    if (parent != NULL || tag == "window") {
        attributeManager->refreshProfile();
    }
}

/* END OF STYLE */