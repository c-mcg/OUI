#include "event/OUI_EventDispatcher.h"
#include "components/OUI_Component.h"

void oui::EventDispatcher::dispatchEvent(Component* target, ComponentEvent event) {
    
    // TODO add when method is supported
    // target->handleEvent(event);

    if (event.isPropagationStopped()) {
        return;
    }

    Component* parent = (Component*) target->getParent();
    if (parent != NULL) {
        event.setTarget(parent);
        this->dispatchEvent(parent, event);
    }
}
