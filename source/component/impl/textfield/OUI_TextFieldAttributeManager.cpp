#include "component/impl/textfield/OUI_TextFieldAttributeManager.h"
#include "component/OUI_Component.h"

oui::TextFieldAttributeManager::TextFieldAttributeManager():
    text{text}, font{font}, textColor{Color::BLACK}, caratWidth{0},
    caratColor{Color::BLACK}, caratHeightOffset{0}, highlightColor{Color::WHITE},
    ComponentAttributeManager()
{
    std::unordered_map<std::string, AttributeVariableInfo> variableMap({
        { "text", {AttributeManager::STRING, &text} },
        { "font", {AttributeManager::FONT, &font} },
        { "font-face", {AttributeManager::FONT, &font} },
        { "font-size", {AttributeManager::FONT, &font} },
        { "text-color", {AttributeManager::COLOR, &textColor} },
        { "carat-width", {AttributeManager::INT, &caratWidth} },
        { "carat-color", {AttributeManager::COLOR, &caratColor} },
        { "carat-h-offset", {AttributeManager::INT, &caratHeightOffset} },
        { "highlight-color", {AttributeManager::COLOR, &highlightColor} },
    });

    updateVariableMap(variableMap);
}

oui::Color oui::TextFieldAttributeManager::getTextColor() {
    return textColor;
}
oui::Font* oui::TextFieldAttributeManager::getFont() {
    return font;        
}
oui::Color oui::TextFieldAttributeManager::getCaratColor() {
    return caratColor;
}
int oui::TextFieldAttributeManager::getCaratWidth() {
    return caratWidth;
}
int oui::TextFieldAttributeManager::getCaratHeightOffset() {
    return caratHeightOffset;
}
oui::Color oui::TextFieldAttributeManager::getHighlightColor() {
    return highlightColor;
}
std::u16string oui::TextFieldAttributeManager::getText() {
    return text;
}