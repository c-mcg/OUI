#include "attribute/OUI_Style.h"

oui::Style::~Style() {

    std::unordered_map<std::u16string, AttributeProfile*>::iterator it;

    for (it = profiles.begin(); it != profiles.end();) {
        AttributeProfile* p = it->second;
        it = profiles.erase(it);
        delete p;
    }

    profiles.clear();

}

oui::Style::Style() :
    profiles{std::unordered_map<std::u16string, AttributeProfile*>()} {
}

void oui::Style::addProfile(const std::u16string& name, AttributeProfile* profile) {
    std::unordered_map<std::u16string, AttributeProfile*>::iterator it = profiles.find(name);
    if (it != profiles.end()) {
        it->second->combineProfile(profile);
    } else {
        //TODO this should clone, or combining should delete for consistent behaviour
        profiles.insert({name, profile});

        if (name != u"default" && profile->getDefaultProfiles().size() == 0) {
            //profile->addDefaultProfile(getOrCreateProfile(u"default"));
        }
    }
}

bool oui::Style::hasProfile(const std::u16string& profile) {
    std::unordered_map<std::u16string, AttributeProfile*>::iterator it = profiles.find(profile);
    return it != profiles.end();
}

std::vector<std::u16string> oui::Style::getProfileNames() {
    std::vector<std::u16string> keys;
    keys.reserve(profiles.size());
    for(auto it = profiles.begin(); it != profiles.end(); ++it) {
        keys.push_back(it->first);
    }
    return keys;
}

oui::AttributeProfile* oui::Style::getProfile(const std::u16string& profile) {
    std::unordered_map<std::u16string, AttributeProfile*>::iterator it = profiles.find(profile);
    return it == profiles.end() ? NULL : it->second;
}

oui::AttributeProfile* oui::Style::getOrCreateProfile(const std::u16string& profile) {
    AttributeProfile* apf = getProfile(profile);
    if (apf == NULL) {
        apf = new AttributeProfile("...", profile == u"default" ? NULL : getProfile(u"default"));
        profiles.insert({profile, apf});
    }
    return apf;
}

void oui::Style::combineStyle(Style* style, bool overwrite) {

    if (style == NULL) {
        return;
    }

    std::unordered_map<std::u16string, AttributeProfile*>::iterator it = style->profiles.begin();
    while (it != style->profiles.end()) {//Loop through profiles in style

        //Check if profile currently exists
        std::unordered_map<std::u16string, AttributeProfile*>::iterator it2 = profiles.find(it->first);
        if (it2 != profiles.end()) {

            //Combine profiles if it does
            it2->second->combineProfile(it->second, overwrite);

        } else {

            //Add it as a new profile if it doesn't
            addProfile(it->first, it->second->clone());

        }

        ++it;
    }

}

bool oui::Style::equals(Style* other) {

    //Return false if non-existant
    if (other == NULL) {
        return false;
    }

    //Return false if different number of profiles
    if (profiles.size() != other->profiles.size()) {
        return false;
    }

    std::unordered_map<std::u16string, AttributeProfile*>::iterator thisIt = profiles.begin();
    std::unordered_map<std::u16string, AttributeProfile*>::iterator otherIt = other->profiles.begin();

    while (thisIt != profiles.end()) {

        if (!thisIt->second->equals(otherIt->second)) {
            return false;
        }

        ++thisIt;
        ++otherIt;
    }

    return true;
}

oui::Style* oui::Style::clone() {

    Style* clone = new Style();

    std::unordered_map<std::u16string, AttributeProfile*>::iterator it = profiles.begin();
    while (it != profiles.end()) {
        clone->addProfile(it->first, it->second->clone());

        ++it;
    }

    return clone;

}