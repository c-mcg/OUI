#include "components/OUI_TextFieldAttributeManager.h"
#include "components/OUI_Component.h"

oui::TextFieldAttributeManager::TextFieldAttributeManager():
    text{text}, font{font}, textColor{Color::BLACK}, caratWidth{0},
    caratColor{Color::BLACK}, caratHeightOffset{0}, highlightColor{Color::BLACK},
    ComponentAttributeManager()
{

}

void oui::TextFieldAttributeManager::setProfile(const std::u16string& profileName) {
    ComponentAttributeManager::setProfile(profileName);

    AttributeProfile* profile = style->getProfile(profileName);
    if (profile != NULL) {

        //Text
        text = profile->getString("text");

        //Font
        font = Font::getFont(profile->getString("font-face"), profile->getInt("font-size"), component->getWindow());

        //Text-color
        textColor =Color(profile->getInt("text-color-r"), profile->getInt("text-color-g"), profile->getInt("text-color-b"), profile->getInt("text-color-a"));

        //Carat-width
        caratWidth = profile->getInt("carat-width");
        //Carat-color
        caratColor = Color(profile->getInt("carat-color-r"), profile->getInt("carat-color-g"), profile->getInt("carat-color-b"), profile->getInt("carat-color-a"));
        //Carat-h-offset
        caratHeightOffset = profile->getInt("carat-h-offset");

        //highlight-color
        highlightColor = Color(profile->getInt("highlight-color-r"), profile->getInt("highlight-color-g"), profile->getInt("highlight-color-b"), profile->getInt("highlight-color-a"));

    }
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