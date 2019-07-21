#include "event/OUI_WindowEventDispatcher.h"
#include "components/OUI_Component.h"

oui::WindowEventDispatcher::WindowEventDispatcher() : EventDispatcher() {}

void oui::WindowEventDispatcher::dispatchEvent(ComponentEvent* event) {

    triggerSystemListeners(event);

    if (event->getTarget() != this->target) {
        target->getEventDispatcher()->dispatchEvent(event);
    }
}
