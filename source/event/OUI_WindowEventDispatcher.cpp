#include "event/OUI_WindowEventDispatcher.h"
#include "components/OUI_Component.h"

oui::WindowEventDispatcher::WindowEventDispatcher(): EventDispatcher() {}

void oui::WindowEventDispatcher::dispatchEvent(ComponentEvent* event) {
    if (event->getTarget() != this->target) {
        if (event->getTarget() == NULL) {
            event->setTarget(target);
        }
        event->getTarget()->getEventDispatcher()->dispatchEvent(event);
        return;
    }

    EventDispatcher::dispatchEvent(event);
}
