#include "components/OUI_LabelAttributeManager.h"

#include "components/OUI_Component.h"

oui::LabelAttributeManager::LabelAttributeManager(): 
    autoSize{false}, text{u""}, font{NULL}, textColor{Color::BLACK},
    ComponentAttributeManager()
{

}

void oui::LabelAttributeManager::setProfile(const std::u16string& profileName)  {
    AttributeProfile* profile = style->getProfile(profileName);
    if (profile != NULL) {

        //Auto-size
        autoSize = profile->getBool("auto-size");

        text = profile->getString("text");

        //Font
        font = Font::getFont(profile->getString("font-face"), profile->getInt("font-size"), component->getWindow());

        //Text-color
        textColor = Color(profile->getInt("text-color-r"), profile->getInt("text-color-g"), profile->getInt("text-color-b"), profile->getInt("text-color-a"));

    }

    if (autoSize) {
        parseAttribute("size", u"0 0 " + intToString(font->getStringWidth(text)) + u" " + intToString(font->getStringHeight(text)));
    }

    ComponentAttributeManager::setProfile(profileName);
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