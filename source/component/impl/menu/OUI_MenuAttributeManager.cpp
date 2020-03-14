#include "component/impl/menu/OUI_MenuAttributeManager.h"
#include "component/impl/menu/OUI_Menu.h"

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

    // //Options
    std::vector<std::u16string> newOptions = getOptions();
    if (areOptionsDifferent(oldOptions, newOptions)) {
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
        auto oldOptions = getOptions();
        auto newOptions = value.asStringArray();
        if (areOptionsDifferent(oldOptions, newOptions)) {
            setOptions(newOptions);
        }
    }

    ContainerAttributeManager::updateAttributeVariable(attributeName, value);
}
            
void oui::MenuAttributeManager::setOptions(std::vector<std::u16string> options) {
    Menu* menu = static_cast<Menu*>(component);
    this->options = options;

    menu->setOptions(this->options);
}

bool oui::MenuAttributeManager::areOptionsDifferent(std::vector<std::u16string> oldOptions, std::vector<std::u16string> newOptions) {
    if (newOptions.size() != oldOptions.size()) {
        return true;
    } else {
        for (int i = 0; i < newOptions.size(); i++) {
            if (newOptions[i] != oldOptions[i]) {
                return true;
            }
        }
    }

    return false;
}
