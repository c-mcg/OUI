#ifndef OUI_ATTRIBUTESUBSTITUTION_H
#define OUI_ATTRIBUTESUBSTITUTION_H

#include "OUI_Export.h"

#include "osal/OSAL_Attribute.h"
#include <vector>
#include <unordered_map>

namespace oui {

    //Forward declare this so it's usable
    class AttributeProfile;

    //An instance of this class holds data to represent attribute substitutions
    //This class also holds a static list of all the substitutions
    //It manages adding new substitutions and performs the substitution processing
    class AttributeSubstitution {

//Static members and functions

        //Represents the type of substitution
        public: enum SubstitutionType {
            APPLY_RESPECTIVELY,//Attributes are applied respectively to their substitutions. 
                //E.g if "a" has substitutions "b", "c"
                //Then "a: x y;" becomes "b: x; c: y;"

            APPLY_WHOLE//All attributes are copied for each substitution
                //E.g if "a" has substitutions "b", "c"
                //Then "a: x y;" becomes "b: x y; c: x y;"

        };

        //The current list of substitutions that each parse attribute is run through
        private: static std::unordered_map<std::string, AttributeSubstitution> attributeSubs;

        //Adds a single substitution for the specified attribute
        public: OUI_API static void addSubstitution(const std::string& name, const std::string& replacement, SubstitutionType type = APPLY_RESPECTIVELY);

        //Adds multiple substitutions for the specified attribute
        public: OUI_API static void addSubstitution(const std::string& attribute, const std::vector<std::string>& subAttributes, SubstitutionType type = APPLY_RESPECTIVELY);

        //Returns true if substitution exists, false if not
        public: OUI_API static bool hasSubstitution(const std::string& name);

        //public: static AttributeSubstitution getSubstitution(const std::u16string& name);//This is weird because we need to throw an error

        //Performs a substitution and adds the new attributes to the attribute profile
        public: static bool applySubstitution(const std::string& name, OSAL::Attribute value, AttributeProfile* profile);

		public: OUI_API AttributeSubstitution(SubstitutionType type, std::vector<std::string> substitutions);

//Instance members

        //The list of attributes names that should be substituted
        //E.g if "a" has substitutions "b", and "c"
        //Then this will hold {b, c}
		protected: std::vector<std::string> substitutions;

        //The type of substitution that should be performed
		protected: SubstitutionType type = APPLY_RESPECTIVELY;
    };

}

#endif