#include "component/OUI_ComponentAttributeManager.h"
#include "component/OUI_Component.h"
#include "component/attribute/OUI_AttributeNames.h"

using namespace oui::AttributeNames;

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
        { X_PERCENT, {AttributeManager::INT, &xPercent} },
        { Y_PERCENT, {AttributeManager::INT, &yPercent} },
        { X_OFFSET, {AttributeManager::INT, &xOffset} },
        { Y_OFFSET, {AttributeManager::INT, &yOffset} },
        { Z_INDEX, {AttributeManager::INT, &z} },
        { INTERACTABLE, {AttributeManager::BOOL, &interactable} },
        { MIN_WIDTH, {AttributeManager::INT, &minWidth} },
        { MIN_HEIGHT, {AttributeManager::INT, &minHeight} },
        { WIDTH_PERCENT, {AttributeManager::INT, &widthPercent} },
        { HEIGHT_PERCENT, {AttributeManager::INT, &heightPercent} },
        { WIDTH_OFFSET, {AttributeManager::INT, &widthOffset} },
        { HEIGHT_OFFSET, {AttributeManager::INT, &heightOffset} },
        { BACKGROUND_COLOR_1, {AttributeManager::COLOR, &backgroundColor1} },
        { BACKGROUND_COLOR_2, {AttributeManager::COLOR, &backgroundColor2} },
        { CENTERED_X, {AttributeManager::BOOL, &centeredX} },
        { CENTERED_Y, {AttributeManager::BOOL, &centeredY} },
        { BORDER_COLOR, {AttributeManager::COLOR, &borderColor} },
        { BORDER_WIDTH, {AttributeManager::INT, &borderWidth} },
        { BORDER_STYLE, {AttributeManager::STRING, &borderStyle} },
        { OPACITY, {AttributeManager::INT, &opacity} },
        { VISIBLE, {AttributeManager::BOOL, &visible} },
        { PERMANENT, {AttributeManager::BOOL, &permanent} },
        { RIGHT_CLICK_OPTIONS, {AttributeManager::STRING_ARRAY, &rightClickOptions} },
        { CURSOR, {AttributeManager::STRING, &cursor} },
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