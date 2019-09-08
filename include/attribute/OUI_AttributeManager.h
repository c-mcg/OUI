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

        public: static const char STRING = OSAL::TYPE_STRING;
        public: static const char INT = OSAL::TYPE_INT;
        public: static const char DOUBLE = OSAL::TYPE_DOUBLE;
        public: static const char BOOL = OSAL::TYPE_BOOL;
        public: static const char COLOR = OSAL::TYPE_COLOR;
        public: static const char ARRAY = OSAL::TYPE_ARRAY;
        public: static const char STRING_ARRAY = 1000;
        public: static const char FONT = 1001;
        public: static const char IMAGE = 1002;

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


            virtual void deriveAttributesForComponent(StyleSheet* styleSheet = NULL);

        protected:

            Component* component;

            Style* definedStyle;
            Style* style;

            std::u16string currentProfileName;
            AttributeProfile* currentProfile;

            virtual int getInt(std::string name);
            virtual double getDouble(std::string name);
            virtual std::u16string getString(std::string name);
            virtual bool getBool(std::string name);

            virtual Attribute getAttribute(std::string name, Attribute defaultVal=0);

        private:

            std::unordered_map<std::string, AttributeVariableInfo> variableMap;

    };
}

#endif