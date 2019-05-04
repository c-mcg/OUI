#ifndef OUI_ATTRIBUTEPROFILE_H
#define OUI_ATTRIBUTEPROFILE_H
#include "OUI_Export.h"
#include "OUI_Attribute.h"
#include "OUI_AttributeSubstitution.h"


namespace oui {

    /** 
     * This class represents an attribute profile, for example, "hover" attributes
     * This is used directly for components, as well as in style sheets to represent a generic class or tag
     * This class should be the only place that Attributes should be created or destroyed
     */
    class AttributeProfile {

//Instance members

        /**
         * The profile to get attribute values if they don't exist here
         */
        private: std::vector<AttributeProfile*> defaultProfiles;

        /**
         * This list of this profile's attributes
         */
        private: std::unordered_map<std::string, Attribute> attributes; 

        /**
         * The name of the component this profile applies to (used for debugging only)
         */
        public: std::string componentName;

//Instance functions
        public: OUI_API ~AttributeProfile();
        public: OUI_API AttributeProfile(const std::string& compName, AttributeProfile* defaultProfile = NULL);

        public: OUI_API void clearDefaultProfiles();
        
        public: OUI_API void addDefaultProfile(AttributeProfile* defaultProfile);
        public: OUI_API std::vector<AttributeProfile*> getDefaultProfiles();

        //TODO default attributes values for attributes with optional parameters (bg-color: 0 0 0 128; then bg-color: 0 0 0; will result in 128 opacity)

        //Used to set and parse an attribute from an OSAL string
        public: OUI_API void parseAttribute(const std::string& name, const std::u16string& value);

        //Used to set an attribute from an OSAL value
        public: OUI_API void setAttribute(const std::string& name, OSAL::Attribute value);

        //Used to directly set an attribute
        public: OUI_API void setAttribute(const std::string& name, Attribute value);

        //Returns true if an attribute value exists, false if not
        //Checks default profiles
        //To ignore default profiles, use containsAttribute
        private: OUI_API Attribute* hasAttribute(const std::string& name);

        //Gets the value of an integer attribute
        public: OUI_API Attribute* getAttribute(const std::string& name);

        //Gets the value of an integer attribute
        public: OUI_API int getInt(const std::string& name);
        
        //Gets the value of a string attribute
        public: OUI_API std::u16string getString(const std::string& name);

        //Gets the value of a boolean attribute
        public: OUI_API bool getBool(const std::string& name);

        //Gets the value of a double attribute
        public: OUI_API double getDouble(const std::string& name);

        //Removes an attribute if it exists
        //TODO this should be a bool, and maybe throw some errors in the engine if it doesn't work
        public: OUI_API void removeAttribute(const std::string& name);

        //Adds the specified profile's attributes to this profile
        //Overwrites attributes if `overwrite` is true
        public: OUI_API void combineProfile(AttributeProfile* profile, bool overwrite = false);

        //Returns true if these objects are equal using a shallow (Checks only values not pointers) check
        public: OUI_API bool equals(AttributeProfile* other);

        //Clones the AttributeProfile
        public: OUI_API AttributeProfile* clone();

        private: OUI_API void combineDefaults(AttributeProfile* defaultProfile);

        //Creates a new AttributeProfile will all of the attribute values flattened from default profiles
        public: OUI_API AttributeProfile* flatten();

    };

}

#endif