#include "oui/attribute/OUI_StyleSheet.h"

#include "util/OUI_StringUtil.h"

oui::StyleSheet::~StyleSheet() {
    Style* toDelete = NULL;

    //Delete tags
    for (auto it = tags.begin(); it != tags.end();) {
        toDelete = it->second;
        it = tags.erase(it);
        delete toDelete;
    }

    //Delete classes
    for (auto it = classes.begin(); it != classes.end();) {
        toDelete = it->second;
        it = classes.erase(it);
        delete toDelete;
    }

    //Delete names
    for (auto it = names.begin(); it != names.end();) {
        toDelete = it->second;
        it = names.erase(it);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              delete toDelete;
    }
}

oui::StyleSheet::StyleSheet() :
    tags{std::unordered_map<std::string, Style*>()},
    classes{std::unordered_map<std::string, Style*>()},
    names{std::unordered_map<std::string, Style*>()} {
}

bool oui::StyleSheet::hasStyle(ElementType type, const std::string& identifier) {
    return getStyle(type, identifier) != NULL;
}

oui::Style* oui::StyleSheet::getStyle(ElementType type, const std::string& identifier) {
    switch (type) {
        case ElementType::TAG:
            return getByTag(identifier);
            break;
        case ElementType::CLASS:
            return getByClass(identifier);
            break;
        case ElementType::NAME:
            return getByName(identifier);
            break;
    }
	return NULL;
}

void oui::StyleSheet::addStyle(ElementType type, std::string identifier, Style* style) {
    switch (type) {
        case ElementType::TAG:
            tags.insert({identifier, style});
            break;
        case ElementType::CLASS:
            classes.insert({identifier, style});
            break;
        case ElementType::NAME:
            names.insert({identifier, style});
            break;
    }
}

oui::Style* oui::StyleSheet::getByTag(const std::string& tag) {
    auto it = tags.find(tag);
    if (it != tags.end()) {
        return it->second;
    }
    return NULL;
}
oui::Style* oui::StyleSheet::getByClass(const std::string& className) {
    auto it = classes.find(className);
    if (it != classes.end()) {
        return it->second;
    }
    return NULL;
}
oui::Style* oui::StyleSheet::getByName(const std::string& name) {
    auto it = names.find(name);
    if (it != names.end()) {
        return it->second;
    }
    return NULL;
}
        
oui::StyleSheet* oui::StyleSheet::fromOSAL(OSAL::Sheet osalSheet) {
    StyleSheet* sheet = new StyleSheet();

    std::vector<OSAL::Element> elements = osalSheet.getElements();
    std::vector<OSAL::Element>::iterator eIt = elements.begin();
    while (eIt != elements.end()) {
        //Processing for each element
        //Each element holds a type (tag, class name) and a set of profiles with attributes

        auto profileList = eIt->getAttributes();
        auto pIt = profileList.begin();

        //Find or create a style for the element
        Style* elementStyle = NULL;
        if (sheet->hasStyle(eIt->getType(), eIt->getName())) {
            elementStyle = sheet->getStyle(eIt->getType(), eIt->getName());
        } else {
            elementStyle = new Style();
            sheet->addStyle(eIt->getType(), eIt->getName(), elementStyle);
        }

        while (pIt != profileList.end()) {
            //Processing for each profile in an element

            //Find or create the attribute profile
            AttributeProfile* profile = NULL;
            if (elementStyle->hasProfile(pIt->first)) {
                profile = elementStyle->getProfile(pIt->first);
            } else {
                profile = new AttributeProfile("...");
                elementStyle->addProfile(pIt->first, profile);
            }

            std::vector<OSAL::Attribute> attributes = pIt->second;
            std::vector<OSAL::Attribute>::iterator aIt = attributes.begin();
            while (aIt != attributes.end()) {
                //TODO this should be setAttribute... At the time of writing, OSAL attribute types are unreliable so it must be parsed
                //std::cout << "Parsing attribute: element=" << eIt->getName().c_str() << " " << aIt->getName().c_str() << "=" << convertUTF16toUTF8(aIt->getOriginalString()).c_str() << " type=" << ((int)aIt->getType()) << std::endl;
                profile->setAttribute(aIt->getName(), *aIt);
                ++aIt;
            }

            ++pIt;
        }

        ++eIt;
    }

    return sheet;
}

void oui::StyleSheet::combineStyleSheet(StyleSheet* sheet, bool overwrite) {
    auto it = sheet->tags.begin();
    while (it != sheet->tags.end()) {

        auto it2 = tags.find(it->first);
        if (it2 != tags.end()) {
            it2->second->combineStyle(it->second, overwrite);
        } else {
            tags.insert({it->first, it->second->clone()});
        }

        it++;
    }

    it = sheet->classes.begin();
    while (it != sheet->classes.end()) {

        auto it2 = classes.find(it->first);
        if (it2 != classes.end()) {
            it2->second->combineStyle(it->second, overwrite);
        } else {
             classes.insert({it->first, it->second->clone()});
        }

        it++;
    }

    it = sheet->names.begin();
    while (it != sheet->names.end()) {

        auto it2 = names.find(it->first);
        if (it2 != names.end()) {
            it2->second->combineStyle(it->second, overwrite);
        } else {
            names.insert({it->first, it->second->clone()});
        }

        it++;
    }
}

bool oui::StyleSheet::isEmpty() {
    return tags.empty() && classes.empty() && names.empty();
}

bool oui::StyleSheet::equals(StyleSheet* other) {

    //Return false if non-existant
    if (other == NULL) {
        return false;
    }

    //Return false if list sizes don't match
    if (tags.size() != other->tags.size()
        || classes.size() != other->classes.size()
        || names.size() != other->names.size()) {
        return false;
    }

    //TODO
    auto thisIt = tags.begin();
    auto otherIt = other->tags.begin();

    //Loop through tags
    while (thisIt != tags.end()) {

        //Return false if styles aren't equal
        if (!thisIt->second->equals(otherIt->second)) {
            return false;
        }

        thisIt++;
        otherIt++;
    }

    thisIt = classes.begin();
    otherIt = other->classes.begin();

    //Loop through classes
    while (thisIt != classes.end()) {

        //Return false if styles aren't equal
        if (!thisIt->second->equals(otherIt->second)) {
            return false;
        }

        thisIt++;
        otherIt++;
    }

    thisIt = names.begin();
    otherIt = other->names.begin();

    //Loop through names
    while (thisIt != names.end()) {

        //Return false if styles aren't equal
        if (!thisIt->second->equals(otherIt->second)) {
            return false;
        }

        thisIt++;
        otherIt++;
    }

    return true;
}

oui::StyleSheet* oui::StyleSheet::clone() {

    StyleSheet* clone = new StyleSheet();

    auto it = tags.begin();

    //Loop through tags
    while (it != tags.end()) {

        //Add tag to clone
        clone->addStyle(ElementType::TAG, it->first, it->second->clone());

        it++;
    }

    it = classes.begin();

    //Loop through classes
    while (it != classes.end()) {

        //Add tag to clone
        clone->addStyle(ElementType::CLASS, it->first, it->second->clone());

        it++;
    }

    it = names.begin();

    //Loop through names
    while (it != names.end()) {

        //Add tag to clone
        clone->addStyle(ElementType::NAME, it->first, it->second->clone());

        it++;
    }

    return clone;
}
