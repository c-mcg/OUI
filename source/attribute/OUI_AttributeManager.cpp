#include "attribute/OUI_AttributeManager.h"

#include "attribute/OUI_Style.h"
#include "components/OUI_Component.h"
#include "components/OUI_Container.h"

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

    currentProfileName = profileName;
    currentProfile = profile;
}

void oui::AttributeManager::refreshProfile() {
    setProfile(currentProfileName);
}

void oui::AttributeManager::parseAttribute(const std::string& name, const std::u16string& value, const std::u16string& profile) {
    
    style->getOrCreateProfile(profile)->parseAttribute(name, value);
    component->flagProfileUpdate();
}

void oui::AttributeManager::setAttribute(const std::string& name, Attribute a, const std::u16string& profile) {
    style->getOrCreateProfile(profile)->setAttribute(name, a);
    component->flagProfileUpdate();
}

void oui::AttributeManager::updateAttributeVariable(const std::string& attributeName, Attribute value) {
    
    std::string originalName = attributeName;
    std::string name = attributeName;
    int index = -1;
    if (stringEndsWith(name, "_length")) { // Is length of an array
        name = name.substr(0, name.length() - 7);
    } else if (name.length() > 2 && name.at(name.length() - 2) == '_') { // Is a value of an array
        try {
            std::string indexStr = name.substr(name.length() - 1, 1);    
            index = std::stoi(indexStr);
            name = name.substr(0, name.length() - 3);
        } catch(std::invalid_argument) {
            index = -1;
        }
    } else if (name.length() > 3 && name.at(name.length() - 3) == '_') { // Is a value of an array
        try {
            std::string indexStr = name.substr(name.length() - 2, 2);    
            index = std::stoi(indexStr);
            name = name.substr(0, name.length() - 3);
        } catch(std::invalid_argument) {
            index = -1;
        }
    }

    auto it = variableMap.find(name);
    if (it == variableMap.end()) {
        // We don't have a local variable
        return;
    }

    AttributeVariableInfo info = it->second;

    if (info.variableType == AttributeManager::STRING) {
        std::u16string* stringValue = static_cast<std::u16string*>(info.variablePointer);
        *stringValue = value.stringVal;
    } else if (info.variableType == AttributeManager::INT) {
        int* intValue = static_cast<int*>(info.variablePointer);
        *intValue = value.intVal;

        if (name == "opacity") {
            Graphics* graphics = component->getGraphics();
            if (graphics != NULL) {
                graphics->setAlpha(*intValue);
            }
        }
    } else if (info.variableType == AttributeManager::BOOL) {
        bool* boolValue = static_cast<bool*>(info.variablePointer);
        *boolValue = value.boolVal;
    } else if (info.variableType == AttributeManager::DOUBLE) {
        double* doubleValue = static_cast<double*>(info.variablePointer);
        *doubleValue = value.doubleVal;
    } else if (info.variableType == AttributeManager::COLOR) {
        Color* colorVal = static_cast<Color*>(info.variablePointer);
        *colorVal = value.colorVal;
    }
            
    if (info.variableType == AttributeManager::STRING_ARRAY) {
        std::vector<std::u16string>* stringArrayVal = static_cast<std::vector<std::u16string>*>(info.variablePointer);
        
        if (index != -1) {
            (*stringArrayVal)[index] = value.stringVal;
        } else if(stringEndsWith(originalName, "_length")) {
            stringArrayVal->resize(value.intVal);
        }
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

    component->flagProfileUpdate();
    this->style = currentStyle;
}
