#ifndef OUI_CONTAINER_ATTRIBUTE_MANAGER_H
#define OUI_CONTAINER_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "component/OUI_ComponentAttributeManager.h"

namespace oui {

    class Container;

    class OUI_API ContainerAttributeManager : public ComponentAttributeManager {
    
        public:

            ~ContainerAttributeManager();
            ContainerAttributeManager();

            virtual void refreshProfile() override;

            virtual void deriveAttributesForComponent(StyleSheet* styleSheet = NULL) override;

            StyleSheet* getStyleSheet();
            void addStyleSheet(StyleSheet* sheet);
            virtual StyleSheet* getAllStyleSheets();

            virtual void updateAttributeVariable(const std::string& attributeName, Attribute value) override;

        protected:
        
            StyleSheet* styleSheet;

            void setStyleSheet(StyleSheet* sheet);


    };

}

#endif