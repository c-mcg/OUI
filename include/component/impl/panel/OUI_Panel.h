#ifndef OUI_PANEL_H
#define OUI_PANEL_H

#include "OUI_Export.h"
#include "component/impl/container/OUI_Container.h"

namespace oui {

    class OUI_API Panel : public Container {

        public:
        
            ~Panel();
            Panel(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher=new EventDispatcher(), ContainerAttributeManager* attributeManager=new ContainerAttributeManager());

    };
}
#endif
