#include "attribute/OUI_AttributeManager.h"

#include "attribute/OUI_Style.h"
#include "components/OUI_Component.h"
#include "components/container/OUI_Container.h"
#include "attribute/OUI_AttributeNames.h"

using namespace oui::AttributeNames;

oui::AttributeManager::~AttributeManager() {
    delete style;
    delete definedStyle;
} 


oui::AttributeManager::AttributeManager(): 
    component{NULL}, definedStyle{new Style()}, style{new Style()},
    variableMap{std::unordered_map<std::string, AttributeVariableInfo>()},
    currentProfileName{u"default"}, currentProfile{NULL}
{

}

void oui::AttributeManager::setComponent(Component* component) {
    if (this->component != NULL) {
        return;
    }
    this->component = component;

    style->getOrCreateProfile(u"default")->componentName = this->component->getName() + ":default";
    style->getOrCreateProfile(u"hover")->componentName = this->component->getName() + ":hover";

    this->currentProfile = style->getProfile(u"default");
}

void oui::AttributeManager::updateVariableMap(std::unordered_map<std::string, AttributeVariableInfo> variableMap) {
    for (auto it = variableMap.begin(); it != variableMap.end(); it++) {
        this->variableMap.insert_or_assign(it->first, it->second);
    }
}

void oui::AttributeManager::setProfile(const std::u16string& profileName) {
    AttributeProfile* profile = style->getProfile(profileName);

    if (profile == NULL) {
        // TODO either error or create the profile
        return;
    }

    auto oldProfile = currentProfile;

    currentProfileName = profileName;
    currentProfile = profile;

    if (currentProfile != oldProfile) {
        refreshProfile();
    }
}

void oui::AttributeManager::refreshProfile() {
    auto it = variableMap.begin();
    for (; it != variableMap.end(); it++) {
        std::string name = it->first;
        std::vector<std::string> substituteNames = AttributeSubstitution::getSubtituteNames(name);
        for (auto it = substituteNames.begin(); it != substituteNames.end(); it++) {
            std::string substituteName = *it;
            Attribute value = getAttribute(substituteName);
            updateAttributeVariable(substituteName, value);
        }
    }
}

void oui::AttributeManager::parseAttribute(const std::string& name, const std::u16string& value, const std::u16string& profile) {
    style->getOrCreateProfile(profile)->parseAttribute(name, value);

    if (profile == currentProfileName) {
        std::vector<std::string> substituteNames = AttributeSubstitution::getSubtituteNames(name);
        for (auto it = substituteNames.begin(); it != substituteNames.end(); it++) {
            std::string substituteName = *it;
            updateAttributeVariable(substituteName, getAttribute(substituteName));
        }
    }
}

void oui::AttributeManager::setAttribute(const std::string& name, Attribute a, const std::u16string& profile) {
    style->getOrCreateProfile(profile)->setAttribute(name, a);

    if (profile == currentProfileName) {
        updateAttributeVariable(name, a);
    }
}

void oui::AttributeManager::updateAttributeVariable(const std::string& attributeName, Attribute value) {
    
    std::string originalName = attributeName;
    std::string name = attributeName;

    auto it = variableMap.find(name);
    if (it == variableMap.end()) {
        // We don't have a local variable
        return;
    }

    AttributeVariableInfo info = it->second;

    if (info.variableType == AttributeManager::STRING) {
        std::u16string* stringValue = static_cast<std::u16string*>(info.variablePointer);
        *stringValue = value.type == OSAL::TYPE_STRING ? value.asString() : u"";
    } else if (info.variableType == AttributeManager::INT) {
        int* intValue = static_cast<int*>(info.variablePointer);
        *intValue = value.type == OSAL::TYPE_INT ? value.asInt() : 0;
    } else if (info.variableType == AttributeManager::BOOL) {
        bool* boolValue = static_cast<bool*>(info.variablePointer);
        *boolValue = value.type == OSAL::TYPE_BOOL ? value.asBool() : false;
    } else if (info.variableType == AttributeManager::DOUBLE) {
        double* doubleValue = static_cast<double*>(info.variablePointer);
        *doubleValue = value.type == OSAL::TYPE_DOUBLE ? value.asDouble() : 0.0;
    } else if (info.variableType == AttributeManager::COLOR) {
        Color* colorVal = static_cast<Color*>(info.variablePointer);
        *colorVal = value.type == OSAL::TYPE_COLOR ? value.asColor() : Color::BLACK;
    } else if (info.variableType == AttributeManager::STRING_ARRAY) {
        std::vector<std::u16string>* stringArrayVal = static_cast<std::vector<std::u16string>*>(info.variablePointer);
        *stringArrayVal = value.asStringArray();
    } else if (info.variableType == AttributeManager::IMAGE) {
        Image** imageValue = static_cast<Image**>(info.variablePointer);
        if (component->getWindow() != NULL) {
            std::u16string imagePath = value.asString();
            *imageValue = Image::loadImage(imagePath, component->getWindow());
        }
    } else if (info.variableType == AttributeManager::FONT) {
        Font** fontValue = static_cast<Font**>(info.variablePointer);
        std::u16string fontName = attributeName == FONT_FACE ? value.asString() : getString(FONT_FACE);
        int fontSize = attributeName == FONT_SIZE ? value.asInt() : getInt(FONT_SIZE);
        *fontValue = Font::getFont(fontName, fontSize, component->getWindow());
    }

}

int oui::AttributeManager::getInt(std::string name) {
    return currentProfile->getInt(name);
}
double oui::AttributeManager::getDouble(std::string name) {
    return currentProfile->getDouble(name);
}
std::u16string oui::AttributeManager::getString(std::string name) {
    return currentProfile->getString(name);
}
bool oui::AttributeManager::getBool(std::string name) {
    return currentProfile->getBool(name);
}

oui::Attribute oui::AttributeManager::getAttribute(std::string name, Attribute defaultVal) {
    Attribute* attribute = currentProfile->getAttribute(name);
    if (attribute == NULL) {
        return defaultVal;
    }
    return *attribute;
}


void oui::AttributeManager::deriveAttributesForComponent(StyleSheet* styleSheet) {
    Style* currentStyle = style;

    Container* parent = component->getParent();
    if(styleSheet != NULL || parent != NULL) {

        if (styleSheet == NULL) {
            styleSheet = parent->getAllStyleSheets();

            if (styleSheet == NULL) {
                return;
            }
        }

        //Apply tag styling
        Style* attributesByTag = styleSheet->getByTag(component->getTag());
        currentStyle->combineStyle(attributesByTag, true);


        //Apply class styling
        auto classes = component->getClasses();
        auto it = classes.begin();
        for (; it != classes.end(); it++) {
            Style* attributesByClass = styleSheet->getByClass(*it);
            currentStyle->combineStyle(attributesByClass, true);
        }

        //Apply name styling
        Style* attributesByName = styleSheet->getByName(component->getName());
        currentStyle->combineStyle(attributesByName, true);
    }

    // Style* optimizedStyle = new Style();
    // auto profileNames = currentStyle->getProfileNames();
    // for(auto it = profileNames.begin(); it != profileNames.end(); ++it) {
    //     std::u16string profileName = *it;
    //     AttributeProfile* optimizedProfile = currentStyle->getProfile(profileName)->flatten();
    //     optimizedStyle->addProfile(profileName, optimizedProfile);
    // }

    //currentStyle->combineStyle(definedStyle);

    refreshProfile();
    this->style = currentStyle;
}
