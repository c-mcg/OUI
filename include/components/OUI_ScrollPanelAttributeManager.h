#ifndef OUI_SCROLL_PANEL_ATTRIBUTE_MANAGER_H
#define OUI_SCROLL_PANEL_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "components/OUI_ContainerAttributeManager.h"

namespace oui {

    class Font;

    class OUI_API ScrollPanelAttributeManager : public ContainerAttributeManager {
    
        public:

            ScrollPanelAttributeManager();

            virtual void refreshProfile() override;
            
    };

}

#endif