#include "components/OUI_ButtonAttributeManager.h"
#include "components/OUI_Button.h"

oui::ButtonAttributeManager::ButtonAttributeManager():
    link{u""}, image{NULL}, textColor{Color::BLACK}, font{NULL},
    centerText{false}, text{u""},
    ComponentAttributeManager(Button::getDefaultButtonStyle())
{
    std::unordered_map<std::string, AttributeVariableInfo> variableMap({
        { "link", {AttributeManager::STRING, &link} },
        { "image", {AttributeManager::IMAGE, &image} },
        { "text-color", {AttributeManager::COLOR, &textColor} },
        { "font", {AttributeManager::FONT, &font} },
        { "font-face", {AttributeManager::FONT, &font} },
        { "font-size", {AttributeManager::FONT, &font} },
        { "center-text", {AttributeManager::BOOL, &centerText} },
        { "text", {AttributeManager::STRING, &text} },
    });

    updateVariableMap(variableMap);
}

std::u16string oui::ButtonAttributeManager::getLink() {
    return link;
}
oui::Color oui::ButtonAttributeManager::getTextColor() {
    return textColor;
}
oui::Image* oui::ButtonAttributeManager::getImage() {
    return image;
}
oui::Font* oui::ButtonAttributeManager::getFont() {
    return font;
}
bool oui::ButtonAttributeManager::isTextCentered() {
    return centerText;
}
std::u16string oui::ButtonAttributeManager::getText() {
    return text;
}
