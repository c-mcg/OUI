#ifndef OUI_EVENT_DISPATCHER_H
#define OUI_EVENT_DISPATCHER_H

#include "OUI_Export.h"
#include "event/OUI_ComponentEvent.h"

namespace oui {

    class Component;

    class OUI_API EventDispatcher {
        public:

            EventDispatcher();
            void dispatchEvent(Component* target, ComponentEvent event);
    };

}
#endif