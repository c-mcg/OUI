#include "components/OUI_ComponentAttributeManager.h"
#include "components/OUI_Component.h"

oui::ComponentAttributeManager::ComponentAttributeManager(Style* defaultStyle): 
    visible{true}, interactable{true}, opacity{255}, backgroundColor1{Color::WHITE},
    backgroundColor2{Color::WHITE}, cursor{Component::CURSOR_DEFAULT}, borderStyle{Component::BORDER_NONE},
    borderWidth{1}, borderColor{Color::BLACK}, centeredX{false}, centeredY{false},
    xPercent{0}, yPercent{0}, xOffset{0}, yOffset{0}, z{0}, minWidth{0}, minHeight{0},
    widthPercent{0}, heightPercent{0}, widthOffset{0}, heightOffset{0},
    AttributeManager()
{
    if (defaultStyle == NULL) {
        defaultStyle = Component::getDefaultComponentStyle();
    }

    AttributeProfile* defaultProfile = style->getOrCreateProfile(u"default");
    AttributeProfile* hoverProfile = style->getOrCreateProfile(u"hover");

    defaultProfile->clearDefaultProfiles();
    defaultProfile->addDefaultProfile(defaultStyle->getOrCreateProfile(u"default"));

    hoverProfile->clearDefaultProfiles();
    hoverProfile->addDefaultProfile(defaultStyle->getOrCreateProfile(u"hover"));
    hoverProfile->addDefaultProfile(defaultProfile);

    std::unordered_map<std::string, AttributeVariableInfo> variableMap({
        { "x-percent", {AttributeManager::INT, &xPercent} },
        { "y-percent", {AttributeManager::INT, &yPercent} },
        { "x-offset", {AttributeManager::INT, &xOffset} },
        { "y-offset", {AttributeManager::INT, &yOffset} },
        { "z-index", {AttributeManager::INT, &z} },
        { "interactable", {AttributeManager::BOOL, &interactable} },
        { "minWidth", {AttributeManager::INT, &minWidth} },
        { "minHeight", {AttributeManager::INT, &minHeight} },
        { "widthPercent", {AttributeManager::INT, &widthPercent} },
        { "heightPercent", {AttributeManager::INT, &heightPercent} },
        { "widthOffset", {AttributeManager::INT, &widthOffset} },
        { "heightOffset", {AttributeManager::INT, &heightOffset} },
        { "backgroundColor1", {AttributeManager::COLOR, &backgroundColor1} },
        { "backgroundColor2", {AttributeManager::COLOR, &backgroundColor2} },
        { "centeredX", {AttributeManager::BOOL, &centeredX} },
        { "centeredY", {AttributeManager::BOOL, &centeredY} },
        { "borderColor", {AttributeManager::COLOR, &borderColor} },
        { "borderWidth", {AttributeManager::INT, &borderWidth} },
        { "borderStyle", {AttributeManager::STRING, &borderStyle} },
        { "opacity", {AttributeManager::INT, &opacity} },
        { "visible", {AttributeManager::BOOL, &visible} },
        { "right-click-options", {AttributeManager::STRING_ARRAY, &rightClickOptions} },
        { "cursor", {AttributeManager::STRING, &cursor} },
    });

    // AttributeManager::updateVariableMap(variableMap);
}

void oui::ComponentAttributeManager::setProfile(const std::u16string& profileName) {
    AttributeManager::setProfile(profileName);
    AttributeProfile* profile = style->getProfile(profileName);
    Graphics* graphics = component->getGraphics();
    if (profile != NULL) {

        //Location
        xPercent = profile->getInt("x-percent");
        yPercent = profile->getInt("y-percent");
        xOffset = profile->getInt("x-offset");
        yOffset = profile->getInt("y-offset");

        //Z-index
        z = profile->getInt("z-index");

        //Interactable
        interactable = profile->getBool("interactable");

        //Size
        minWidth = profile->getInt("min-width");
        minHeight = profile->getInt("min-height");
        widthPercent = profile->getInt("width-percent");
        heightPercent = profile->getInt("height-percent");
        widthOffset = profile->getInt("width-offset");
        heightOffset = profile->getInt("height-offset");

        //Background
        backgroundColor1 = profile->getColor("bg-color1");
        backgroundColor2 = profile->getColor("bg-color2");

        //Centered
        //TODO substitute and stuff
        centeredX = profile->getBool("centered-x");
        centeredY = profile->getBool("centered-y");

        //Border
        borderColor = profile->getColor("border-color");
        borderWidth = profile->getInt("border-width");
        std::u16string borderStyle = profile->getString("border-style");
        if(borderStyle == u"none") {
            this->borderStyle = Component::BORDER_NONE;
        } else if (borderStyle == u"solid") {
            this->borderStyle = Component::BORDER_SOLID;
        }

        //Opacity
        //TODO remove opacity var
        opacity = profile->getInt("opacity");
        if (graphics != NULL) {
            graphics->setAlpha(opacity);
        }

        //Visible
        visible = profile->getBool("visible");

        //Right-click-options
        rightClickOptions = profile->getStringArray("right-click-options");

        std::u16string cursor = profile->getString("cursor");
        if (cursor == u"default") {
            this->cursor = Component::CURSOR_DEFAULT;
        } else if (cursor == u"pointer") {
            this->cursor = Component::CURSOR_POINTER;
        } else if (cursor == u"text") {
            this->cursor = Component::CURSOR_TEXT;
        }

    }

    component->updateSize();
}

// Behaviour
bool oui::ComponentAttributeManager::isVisible() {
    return visible;
}
bool oui::ComponentAttributeManager::isInteractable() {
    return interactable;
}

// Styling
int oui::ComponentAttributeManager::getOpacity() {
    return opacity;
}
oui::Color oui::ComponentAttributeManager::getBackgroundColor1() {
    return backgroundColor1;
}
oui::Color oui::ComponentAttributeManager::getBackgroundColor2() {
    return backgroundColor2;
}
std::u16string oui::ComponentAttributeManager::getCursor() {
    return cursor;
}

std::u16string oui::ComponentAttributeManager::getBorderStyle() {
    return borderStyle;
}
int oui::ComponentAttributeManager::getBorderWidth() {
    return borderWidth;
}

oui::Color oui::ComponentAttributeManager::getBorderColor() {
    return borderColor;
}

bool oui::ComponentAttributeManager::getCenteredX() {
    return centeredX;
}
bool oui::ComponentAttributeManager::getCenteredY() {
    return centeredY;
}

int oui::ComponentAttributeManager::getXPercent() {
    return xPercent;
}
int oui::ComponentAttributeManager::getYPercent() {
    return yPercent;
}
int oui::ComponentAttributeManager::getXOffset() {
    return xOffset;
}
int oui::ComponentAttributeManager::getYOffset() {
    return yOffset;
}
int oui::ComponentAttributeManager::getZ() {
    return z;
}

// Size
int oui::ComponentAttributeManager::getMinWidth() {
    return minWidth;
}
int oui::ComponentAttributeManager::getMinHeight() {
    return minHeight;
}
int oui::ComponentAttributeManager::getWidthPercent() {
    return widthPercent;
}
int oui::ComponentAttributeManager::getHeightPercent() {
    return heightPercent;
}
int oui::ComponentAttributeManager::getWidthOffset() {
    return widthOffset;
}
int oui::ComponentAttributeManager::getHeightOffset() {
    return heightOffset;
}

std::vector<std::u16string> oui::ComponentAttributeManager::getRightClickOptions() {
    return rightClickOptions;
}