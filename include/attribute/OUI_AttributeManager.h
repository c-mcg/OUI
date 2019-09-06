#ifndef OUI_ATTRIBUTE_MANAGER_H
#define OUI_ATTRIBUTE_MANAGER_H

#include <unordered_map>

#include "OUI_Export.h"
#include "attribute/OUI_Attribute.h"

namespace oui {

    class Component;
    class Style;
    class AttributeProfile;
    class StyleSheet;

    struct AttributeVariableInfo {
        char variableType;
        void* variablePointer;
    };

    class OUI_API AttributeManager {

        public: static const char STRING = 0;
        public: static const char INT = 1;
        public: static const char DOUBLE = 2;
        public: static const char BOOL = 3;
        public: static const char COLOR = 4;
        public: static const char STRING_ARRAY = 5;

        public:

            ~AttributeManager();
            AttributeManager();

            void setComponent(Component* component);

            void updateVariableMap(std::unordered_map<std::string, AttributeVariableInfo> variableMap);

            virtual void setProfile(const std::u16string& profile);
            virtual void refreshProfile();

            virtual void parseAttribute(const std::string& name, const std::u16string& value, const std::u16string& profile=u"default");
            virtual void setAttribute(const std::string& name, Attribute a, const std::u16string& profile=u"default");
            virtual void updateAttributeVariable(const std::string& name, Attribute value);

            virtual int getInt(std::string name);
            virtual double getDouble(std::string name);
            virtual std::u16string getString(std::string name);
            virtual bool getBool(std::string name);

            virtual Attribute getAttribute(std::string name, Attribute defaultVal=0);

            virtual void deriveAttributesForComponent(StyleSheet* styleSheet = NULL);

        protected:

            Component* component;

            Style* definedStyle;
            Style* style;

        private:

            std::unordered_map<std::string, AttributeVariableInfo> variableMap;

            std::u16string currentProfileName;
            AttributeProfile* currentProfile;
        
        

    };
}

#endif