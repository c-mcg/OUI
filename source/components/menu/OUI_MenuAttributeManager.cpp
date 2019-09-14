#include "components/menu/OUI_MenuAttributeManager.h"
#include "components/menu/OUI_Menu.h"

oui::MenuAttributeManager::MenuAttributeManager():
    hoverColor{Color::WHITE}, padding{0}, minWidth{0}, optionHeight{0},
    fontName{u""}, fontSize{0},
    ContainerAttributeManager()
{
    std::unordered_map<std::string, AttributeVariableInfo> variableMap({
        { "hover-color", {AttributeManager::COLOR, &hoverColor} },
        { "padding", {AttributeManager::INT, &padding} },
        { "min-width", {AttributeManager::INT, &minWidth} },
        { "option-height", {AttributeManager::INT, &optionHeight} },
        { "font-face", {AttributeManager::STRING, &fontName} },
        { "font-size", {AttributeManager::INT, &fontSize} },
        { "options", {AttributeManager::STRING_ARRAY, &options} },
    });

    updateVariableMap(variableMap);
}

void oui::MenuAttributeManager::refreshProfile() {
    auto oldOptions = getOptions();
    ContainerAttributeManager::refreshProfile();

    //Options
    bool optionsChanged = false;
    std::vector<std::u16string> newOptions = getOptions();
    if (newOptions.size() != oldOptions.size()) {
        optionsChanged = true;
    } else {
        for (int i = 0; i < newOptions.size(); i++) {
            if (newOptions[i] != oldOptions[i]) {
                optionsChanged = true;
                break;
            }
        }
    }
    if (optionsChanged) {
        setOptions(newOptions);
    }
    
    ContainerAttributeManager::refreshProfile();
}

int oui::MenuAttributeManager::getNumOptions() {
    return options.size();
}
std::vector<std::u16string> oui::MenuAttributeManager::getOptions() {
    return options;
}

std::u16string oui::MenuAttributeManager::getFontName() {
    return fontName;
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

void oui::MenuAttributeManager::updateAttributeVariable(const std::string& attributeName, Attribute value) {
    if (attributeName == "options") {
        setOptions(value.asStringArray());
    }

    ContainerAttributeManager::updateAttributeVariable(attributeName, value);
}
            
void oui::MenuAttributeManager::setOptions(std::vector<std::u16string> options) {
    Menu* menu = static_cast<Menu*>(component);
    this->options = options;

    // menu->removeAllOptionComponents();
    // menu->addOptions(this->options);
    // int borderWidth = getBorderWidth();
    // int biggestWidth = menu->resetOptions();
    // parseAttribute("size", u"0 0 " + intToString(biggestWidth + padding * 2 + borderWidth * 2) + u" " + intToString(optionHeight * getNumOptions() + padding * 2 + borderWidth * 2));
}
