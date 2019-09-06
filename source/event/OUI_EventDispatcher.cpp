#include "event/OUI_EventDispatcher.h"
#include "components/OUI_Component.h"
#include "components/OUI_Container.h"

oui::EventDispatcher::EventDispatcher() :
    target{NULL}, listeners() {

}

void oui::EventDispatcher::setTarget(Component* target) {
    if (this->target != NULL) {
        return;
    }
    this->target = target;
}

void oui::EventDispatcher::addEventListener(std::string type, EventHandler handler) {
    auto it = listeners.find(type);

    if (listeners.count(type)) {
        listeners[type].push_back(handler);
    } else {
        std::vector<EventHandler> handlers;
        handlers.push_back(handler);
        listeners[type] = handlers;
    }
}

void oui::EventDispatcher::dispatchEvent(ComponentEvent* event) {
	event->setTarget(target);

    triggerListeners(event);

	if (target != NULL) {
		Component* parent = dynamic_cast<Component*>(target->getParent());
		if (parent != NULL && event->bubbles && !event->isPropagationStopped()) {
			if (event->eventClass == "mouse") {
				MouseEvent* mouseEvent = ( MouseEvent*) event;
				event = mouseEvent->createBubbledEvent(mouseEvent);
			}
			event->setTarget(parent);
			parent->getEventDispatcher()->dispatchEvent(event);
		}
	}
}

void oui::EventDispatcher::triggerListeners(ComponentEvent* event) {
    auto listenerIt = listeners.find(event->type);

    if (listenerIt == listeners.end()) {
        return;
    }
    
    std::vector<EventHandler>& handlers = listenerIt->second;
    for (auto it = handlers.begin(); it != handlers.end(); it++) {
		EventHandler handler = (*it);
        handler(event);
    }
}

