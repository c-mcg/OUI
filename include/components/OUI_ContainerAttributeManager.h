#ifndef OUI_CONTAINER_ATTRIBUTE_MANAGER_H
#define OUI_CONTAINER_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "components/OUI_ComponentAttributeManager.h"

namespace oui {

    class Container;

    class OUI_API ContainerAttributeManager : public ComponentAttributeManager {
    
        public:

            ~ContainerAttributeManager();
            ContainerAttributeManager();

            virtual void setProfile(const std::u16string& profile) override;

            virtual void deriveAttributesForComponent(StyleSheet* styleSheet = NULL) override;

            StyleSheet* getStyleSheet();
            void addStyleSheet(StyleSheet* sheet);
            virtual StyleSheet* getAllStyleSheets();

        protected:
        
            StyleSheet* styleSheet;

            void setStyleSheet(StyleSheet* sheet);


    };

}

#endif