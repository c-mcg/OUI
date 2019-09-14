#include "components/OUI_ComponentAttributeManager.h"
#include "components/OUI_Component.h"

oui::ComponentAttributeManager::ComponentAttributeManager(Style* defaultStyle): 
    visible{true}, interactable{true}, permanent{false}, opacity{255}, backgroundColor1{Color::WHITE},
    backgroundColor2{Color::WHITE}, cursor{constants::CURSOR_DEFAULT}, borderStyle{constants::BORDER_NONE},
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
        { "min-width", {AttributeManager::INT, &minWidth} },
        { "min-height", {AttributeManager::INT, &minHeight} },
        { "width-percent", {AttributeManager::INT, &widthPercent} },
        { "height-percent", {AttributeManager::INT, &heightPercent} },
        { "width-offset", {AttributeManager::INT, &widthOffset} },
        { "height-offset", {AttributeManager::INT, &heightOffset} },
        { "bg-color1", {AttributeManager::COLOR, &backgroundColor1} },
        { "bg-color2", {AttributeManager::COLOR, &backgroundColor2} },
        { "centered-x", {AttributeManager::BOOL, &centeredX} },
        { "centered-y", {AttributeManager::BOOL, &centeredY} },
        { "border-color", {AttributeManager::COLOR, &borderColor} },
        { "border-width", {AttributeManager::INT, &borderWidth} },
        { "border-style", {AttributeManager::STRING, &borderStyle} },
        { "opacity", {AttributeManager::INT, &opacity} },
        { "visible", {AttributeManager::BOOL, &visible} },
        { "permanent", {AttributeManager::BOOL, &permanent} },
        { "right-click-options", {AttributeManager::STRING_ARRAY, &rightClickOptions} },
        { "cursor", {AttributeManager::STRING, &cursor} },
    });

    updateVariableMap(variableMap);
}

void oui::ComponentAttributeManager::refreshProfile() {
    AttributeManager::refreshProfile();

    component->flagGraphicsUpdate();
    component->updateSize();
}

// Behaviour
bool oui::ComponentAttributeManager::isVisible() {
    return visible;
}
bool oui::ComponentAttributeManager::isInteractable() {
    return interactable;
}
bool oui::ComponentAttributeManager::isPermanent() {
    return permanent;
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

void oui::ComponentAttributeManager::updateAttributeVariable(const std::string& attributeName, Attribute value) {
    if (attributeName == "opacity") {
        Graphics* graphics = component->getGraphics();
        if (graphics != NULL) {
            graphics->setAlpha(value.asInt());
        }
    }

    AttributeManager::updateAttributeVariable(attributeName, value);
}