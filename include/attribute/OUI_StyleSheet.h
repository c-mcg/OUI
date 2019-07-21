#ifndef OUI_STYLESHEET_H
#define OUI_STYLESHEET_H
#include "OUI_Export.h"
#include "OUI_Style.h"

namespace oui {
    typedef OSAL::Element::Type ElementType;


    //This class represents a complete style sheet that can represent multiple elements with profiles
    //This class is used to store a loaded style sheet, ready to be applied to elements
    class StyleSheet {

        //The lists of elements, organized by type
        private: std::unordered_map<std::string, Style*> tags;
        private: std::unordered_map<std::string, Style*> classes;
        private: std::unordered_map<std::string, Style*> names;

        public: OUI_API ~StyleSheet();
        public: OUI_API StyleSheet();

        //Returns true if the specified type and identifier exits, false if not
        //Identifier is the tag/class/name of the element
        public: OUI_API bool hasStyle(ElementType type, const std::string& identifier);

        //Returns the style assocciated with the specified type and identifier, NULL if not found
        //Identifier is the tag/class/name of the element
        public: OUI_API Style* getStyle(ElementType type, const std::string& identifier);

        //Adds an element of the specified type and itentifier
        //Identifier is the tag/class/name of the element
        public: OUI_API void addStyle(ElementType type, std::string identifier, Style* style);

        //Returns the style associated with the specified tag, NULL if not found
        public: OUI_API Style* getByTag(const std::string& tag);
        //Returns the style associated with the specified class, NULL if not found
        public: OUI_API Style* getByClass(const std::string& className);
        //Returns the style associated with the specified name, NULL if not found
        public: OUI_API Style* getByName(const std::string& name);

        //Adds the new StyleSheet's attributes to this one
        //Any common attributes are overwritten
        public: OUI_API void combineStyleSheet(StyleSheet* sheet, bool overwrite  = false);

        //TODO make a clone function
        
        //Converts an OSAL sheet into a new OUI stylesheet
        public: OUI_API static StyleSheet* fromOSAL(OSAL::Sheet sheet);

        public: OUI_API bool isEmpty();

        public: OUI_API bool equals(StyleSheet* other);

        public: OUI_API StyleSheet* clone();

    };

}

#endif