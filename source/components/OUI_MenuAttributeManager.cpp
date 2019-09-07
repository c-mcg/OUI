#include "components/OUI_MenuAttributeManager.h"
#include "components/OUI_Menu.h"

oui::MenuAttributeManager::MenuAttributeManager():
    hoverColor{Color::WHITE}, padding{0}, minWidth{0}, optionHeight{0},
    font{NULL}, fontSize{0},
    ContainerAttributeManager()
{

}

void oui::MenuAttributeManager::setProfile(const std::u16string& profileName) {
    AttributeProfile* profile = style->getProfile(profileName);
    if (profile != NULL) {

        hoverColor = profile->getColor("hover-color");

        padding = profile->getInt("padding");

        minWidth = profile->getInt("min-width");

        optionHeight = profile->getInt("option-height");

        minWidth = profile->getInt("min-width");

        //Font
        font = profile->getString("font-face");
        fontSize = profile->getInt("font-size");

        //Options
        bool optionsChanged = false;
        std::vector<std::u16string> newOptions = profile->getStringArray("options");
        if (newOptions.size() != options.size()) {
            optionsChanged = true;
        } else {
            for (int i = 0; i < newOptions.size(); i++) {
                if (newOptions[i] != options[i]) {
                    optionsChanged = true;
                    break;
                }
            }
        }
        if (optionsChanged) {
            Menu* menu = static_cast<Menu*>(component);
            setOptions(newOptions);
            numOptions = newOptions.size();
            int borderWidth = profile->getInt("border-width");
            int biggestWidth = menu->resetOptions();
            parseAttribute("size", u"0 0 " + intToString(biggestWidth + padding * 2 + borderWidth * 2) + u" " + intToString(optionHeight * numOptions + padding * 2 + borderWidth * 2));
        }
    }
    
    ContainerAttributeManager::setProfile(profileName);
}

int oui::MenuAttributeManager::getNumOptions() {
    return numOptions;
}
std::vector<std::u16string> oui::MenuAttributeManager::getOptions() {
    return options;
}

std::u16string oui::MenuAttributeManager::getFont() {
    return font;
}
int oui::MenuAttributeManager::getFontSize() {
    return fontSize;
}
int oui::MenuAttributeManager::getMinWidth() {
    return minWidth;
}
int oui::MenuAttributeManager::getOptionHeight() {
    return optionHeight;
}
oui::Color oui::MenuAttributeManager::getHoverColor() {
    return hoverColor;
}
int oui::MenuAttributeManager::getPadding() {
    return padding;
}

            
std::vector<oui::Button*> oui::MenuAttributeManager::setOptions(const std::vector<std::u16string>& options) {
    Menu* menu = static_cast<Menu*>(component);
    this->options = options;
    menu->removeAllOptionComponents();
    numOptions = 0;    
    return menu->addOptions(options);
}
