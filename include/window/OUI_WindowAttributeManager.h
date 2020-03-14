#ifndef OUI_WINDOW_ATTRIBUTE_MANAGER_H
#define OUI_WINDOW_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "component/impl/container/OUI_ContainerAttributeManager.h"

namespace oui {

    class OUI_API WindowAttributeManager : public ContainerAttributeManager {
    
        public:

            WindowAttributeManager();

            virtual void refreshProfile() override;
            
    };

}

#endif