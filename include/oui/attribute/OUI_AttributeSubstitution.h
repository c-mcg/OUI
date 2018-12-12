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
	class OUI_API AttributeSubstitution {

//Static members and functions

		//Represents the type of substitution
		public: enum SubstitutionType {
			APPLY_RESPECTIVELY,//Attributes are applied respectively to their substitutions. 
				//E.g if "a" has substitutions "b", "c"
				//Then "a: x y;" becomes "b: x; c: y;"

			APPLY_WHOLE,//All attributes are copied for each substitution
				//E.g if "a" has substitutions "b", "c"
				//Then "a: x y;" becomes "b: x y; c: x y;"

			APPLY_INDEXED//Attributes are applied respectively to indexed attributes
				//E.g if "a" has subtitution "b"
				//Then "a: x y;" becomes "b_length: 2; b_0: x; b_1: y;"
				//Most likely, the substitution will have the same name ("a" would have substitution "a")
				//This is possible because the new variables have suffixes and will not look for the original substitution again
		};

		//The current list of substitutions that each parse attribute is run through
		private: static std::unordered_map<std::string, AttributeSubstitution> attributeSubs;

		//Adds a single substitution for the specified attribute
		public: static void addSubstitution(std::string attribute, std::string replacement, SubstitutionType type = APPLY_RESPECTIVELY);

		//Adds multiple substitutions for the specified attribute
		public: static void addSubstitution(std::string attribute, std::vector<std::string> subAttributes, SubstitutionType type = APPLY_RESPECTIVELY);

		//Returns true if substitution exists, false if not
		public: static bool hasSubstitution(std::string name);

		//public: static AttributeSubstitution getSubstitution(std::u16string name);//This is weird because we need to throw an error

		//Performs a substitution and adds the new attributes to the attribute profile
		public: static bool applySubstitution(std::string name, OSAL::Attribute value, AttributeProfile* profile);

//Instance members

		//The list of attributes names that should be substituted
		//E.g if "a" has substitutions "b", and "c"
		//Then this will hold {b, c}
		std::vector<std::string> substitutions;

		//The type of substitution that should be performed
		SubstitutionType type;
	};

}

#endif