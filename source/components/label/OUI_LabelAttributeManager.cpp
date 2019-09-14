#include "components/label/OUI_LabelAttributeManager.h"

#include "components/OUI_Component.h"

oui::LabelAttributeManager::LabelAttributeManager(): 
    autoSize{false}, text{u""}, font{NULL}, textColor{Color::BLACK},
    ComponentAttributeManager()
{
    std::unordered_map<std::string, AttributeVariableInfo> variableMap({
        { "auto-size", {AttributeManager::BOOL, &autoSize} },
        { "text", {AttributeManager::STRING, &text} },
        { "font", {AttributeManager::FONT, &font} },
        { "font-face", {AttributeManager::FONT, &font} },
        { "font-size", {AttributeManager::FONT, &font} },
        { "text-color", {AttributeManager::COLOR, &textColor} },
        { "text-color", {AttributeManager::COLOR, &textColor} },
    });

    updateVariableMap(variableMap);
}

void oui::LabelAttributeManager::refreshProfile()  {
    ComponentAttributeManager::refreshProfile();

    if (autoSize && font != NULL) {
        parseAttribute("size", u"0 0 " + intToString(font->getStringWidth(text)) + u" " + intToString(font->getStringHeight(text)));
    }

    ComponentAttributeManager::refreshProfile();
}

oui::Color oui::LabelAttributeManager::getTextColor() {
    return textColor;
}
oui::Font* oui::LabelAttributeManager::getFont() {
    return font;
}
std::u16string oui::LabelAttributeManager::getText() {
    return text;
}
bool oui::LabelAttributeManager::shouldAutoSize() {
    return autoSize;
}