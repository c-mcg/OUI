#include "components/OUI_ButtonAttributeManager.h"
#include "components/OUI_Button.h"

oui::ButtonAttributeManager::ButtonAttributeManager():
    link{u""}, imageString{u""}, textColor{Color::BLACK}, font{NULL},
    centerText{false}, text{u""},
    ComponentAttributeManager(Button::getDefaultButtonStyle())
{
    
}


void oui::ButtonAttributeManager::setProfile(const std::u16string& profileName) {
    ComponentAttributeManager::setProfile(profileName);

    AttributeProfile* profile = style->getProfile(profileName);
    if (profile != NULL) {

        // Image
        imageString = profile->getString("image");

        // Text
        text = profile->getString("text");

        // Font
        font = Font::getFont(profile->getString("font-face"), profile->getInt("font-size"), component->getWindow());

        // Text-color
        textColor = profile->getColor("text-color");

        // Center-text
        centerText = profile->getBool("center-text");

        // Page Link
        link = profile->getString("link");

    }
}


std::u16string oui::ButtonAttributeManager::getLink() {
    return link;
}
std::u16string oui::ButtonAttributeManager::getImageString() {
    return imageString;
}
oui::Color oui::ButtonAttributeManager::getTextColor() {
    return textColor;
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
