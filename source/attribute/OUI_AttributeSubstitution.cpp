#include "attribute/OUI_AttributeSubstitution.h"
#include "attribute/OUI_AttributeProfile.h"
#include "util/OUI_StringUtil.h"

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
        //TODO error multiple subs for same attribute
    }

    //Make sure we've actually got a substitution
    if (subAttributes.size() == 0) {
        //TODO error, we need an actual substitution
    }

    //Add substitution to list
    attributeSubs.insert({name, AttributeSubstitution(type, subAttributes)});
}

bool oui::AttributeSubstitution::hasSubstitution(const std::string& name) {
    return attributeSubs.find(name) != attributeSubs.end();
}

bool oui::AttributeSubstitution::applySubstitution(const std::string& name, OSAL::Attribute value, AttributeProfile* profile) {

    auto subIt = attributeSubs.find(name);

    //Return if substitution doesn't exist
    if (subIt == attributeSubs.end()) {
        return false;
    }

    SubstitutionType substitutionType = subIt->second.type;
    std::vector<std::string> substitutions = subIt->second.substitutions;

    //Indexed substitution
    //Substitutes an array in place of the attribute
    //See `AttributeSubstitution::SubstitutionType` for examples
    if (substitutionType == APPLY_INDEXED) {

        int length = profile->getInt(substitutions.at(0) + "_length");

        //Remove any existing attributes that correspond with this substitution
        for (int i = value.getNumValues(); i < length; i++) {
            profile->removeAttribute(substitutions.at(0) + "_" + std::to_string(i));
        }

        //Set the length attribute
        profile->setAttribute(substitutions.at(0) + "_length", value.getNumValues());

        //Set indexed attributes
        for (int i = 0; i < value.getNumValues(); i++) {

            //Populate new attribute value
            Attribute val = Attribute(); //TODO this little block is starting to look too familiar
            switch (value.getType(i)) {
                case OSAL::TYPE_INT:
                    val = value.getAsInt(i);
                    break;
                case OSAL::TYPE_STRING:
                    val = std::u16string(value.getAsString(i));
                    break;
                case OSAL::TYPE_DOUBLE:
                    val = value.getAsDouble(i);
                    break;
                case OSAL::TYPE_BOOL:
                    val = value.getAsBool(i);
                    break;
            }

            //Set indexed attribute
            profile->setAttribute(substitutions.at(0) + "_" + std::to_string(i), val);
        }
        return true;
    }
    

    //Respective substitution
    //Substitutes multiple attributes respectively to the specified substitutions
    //See `AttributeSubstitution::SubstitutionType` for examples
    if (substitutionType == APPLY_RESPECTIVELY) {

        //Run through the list of attributes to apply respectively
        for (int i = 0; i < substitutions.size() && i < value.getNumValues(); i++) {
                
            //Here is a small optimization
            //This puts the new attribute back to be substituted only if it needs to be
            auto subIt2 = attributeSubs.find(substitutions.at(i));
            if (subIt2 != attributeSubs.end()) {

                std::u16string attributeStringValue = value.getAsString(i);

                //Put quotations back around the attribute if it's a string
                if (value.getType(i) == OSAL::TYPE_STRING) {
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
            Attribute val = Attribute();
            //Popuplating the new attribute, seen this too many times
            switch (value.getType(i)) {
                case OSAL::TYPE_INT:
                    val = value.getAsInt(i);
                    break;
                case OSAL::TYPE_STRING:
                    val = std::u16string(value.getAsString(i));
                    break;
                case OSAL::TYPE_DOUBLE:
                    val = value.getAsDouble(i);
                    break;
                case OSAL::TYPE_BOOL:
                    val = value.getAsBool(i);
                    break;
            }

            //Set the attribute directly
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