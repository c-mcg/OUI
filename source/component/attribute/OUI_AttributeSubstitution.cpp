#include "component/attribute/OUI_AttributeSubstitution.h"
#include "component/attribute/OUI_AttributeProfile.h"
#include "util/OUI_StringUtil.h"
#include "exception/OUI_ArgumentException.h"

std::unordered_map<std::string, oui::AttributeSubstitution> oui::AttributeSubstitution::attributeSubs = std::unordered_map<std::string, oui::AttributeSubstitution>();

oui::AttributeSubstitution::AttributeSubstitution(SubstitutionType type, std::vector<std::string> substitutions) {
	this->type = type;
	this->substitutions = substitutions;
}


void oui::AttributeSubstitution::addSubstitution(const std::string& name, const std::string& replacement, SubstitutionType type) {
    addSubstitution(name, std::vector<std::string>({replacement}), type);
}

void oui::AttributeSubstitution::addSubstitution(const std::string& name, const std::vector<std::string>& subAttributes, SubstitutionType type) {

    auto subIt = attributeSubs.find(name);

    //Check if substitution already exusts
    if (subIt != attributeSubs.end()) {
        throw ArgumentException(
            "AttributeSubstitution",
            "addSubstitution",
            "Tried to overwrite existing substitution",
            "Contact OUI support"
        );
    }

    //Make sure we've actually got a substitution
    if (subAttributes.size() == 0) {
        throw ArgumentException(
            "AttributeSubstitution",
            "addSubstitution",
            "No attributes specified for substitution",
            "Enter attributes to substitute with"
        );
    }

    //Add substitution to list
    attributeSubs.insert({name, AttributeSubstitution(type, subAttributes)});
}

bool oui::AttributeSubstitution::hasSubstitution(const std::string& name) {
    return attributeSubs.find(name) != attributeSubs.end();
}

std::vector<std::string> oui::AttributeSubstitution::getSubtituteNames(const std::string& name) {
    auto subIt = attributeSubs.find(name);

    if (subIt == attributeSubs.end()) {
        return std::vector<std::string>{name};
    }

    return subIt->second.substitutions;
}


bool oui::AttributeSubstitution::applySubstitution(const std::string& name, OSAL::Attribute value, AttributeProfile* profile) {

    auto subIt = attributeSubs.find(name);

    //Return if substitution doesn't exist
    if (subIt == attributeSubs.end()) {
        return false;
    }

    SubstitutionType substitutionType = subIt->second.type;
    std::vector<std::string> substitutions = subIt->second.substitutions;

    //Respective substitution
    //Substitutes multiple attributes respectively to the specified substitutions
    //See `AttributeSubstitution::SubstitutionType` for examples
    if (substitutionType == APPLY_RESPECTIVELY) {

        std::vector<OSAL::Attribute> osalAttributes = value.getAsArray();

        //Run through the list of attributes to apply respectively
        for (int i = 0; i < substitutions.size() && i < osalAttributes.size(); i++) {
            OSAL::Attribute osalAttribute = osalAttributes[i];
                
            //Here is a small optimization
            //This puts the new attribute back to be substituted only if it needs to be
            auto subIt2 = attributeSubs.find(substitutions.at(i));
            if (subIt2 != attributeSubs.end()) {

                std::u16string attributeStringValue = osalAttribute.getAsString();

                //Put quotations back around the attribute if it's a string
                if (osalAttribute.getType() == OSAL::TYPE_STRING) {
                    attributeStringValue = u"\"" + attributeStringValue + u"\"";
                }

                //String value is parsed, this is really what we're trying to avoid
                //TODO maybe we could avoid this (and the setAttribute call) by just running the substitution because we know it's a single attribute
                    //although we don't know the type (I think this matters)
                OSAL::Attribute osalAttribute = OSAL::Attribute(substitutions.at(i), attributeStringValue);

                //Set the attribute with the parsed string
                profile->setAttribute(substitutions.at(i), osalAttribute);
                continue;
            }

            //If the attribute didn't need to be parsed again, we'll just create it now
            Attribute val = Attribute(osalAttribute);
            profile->setAttribute(substitutions.at(i), val);
    
        }
        return true;
    }
        
    //Whole substitution
    //Applies all attributes to each of the specified substitutions
    //See `AttributeSubstitution::SubstitutionType` for examples
    if (substitutionType == APPLY_WHOLE) {

        //Loop through specified substitutions
        for (int i = 0; i < substitutions.size(); i++) {

            //Send the new attribute with the same values to be checked for substitution again
            //TODO this was optimized above, couldn't it be done again?
            profile->setAttribute(substitutions.at(i), value);

        }

        return true;
    }

    //We didn't perform any substitutions
    return false;
}