#include "event/OUI_EventDispatcher.h"
#include "components/OUI_Component.h"

oui::EventDispatcher::EventDispatcher() : target{NULL} {}

void oui::EventDispatcher::setTarget(Component* target) {
    if (this->target == NULL) {
        return;
    }

    this->target = target;
}

void oui::EventDispatcher::addEventListener(std::string type, EventHandler handler) {
    return addEventListener(Event::createTypeHash(type), handler);
}

void oui::EventDispatcher::addEventListener(std::size_t typeHash, EventHandler handler) {
    auto it = listeners.find(typeHash);

    std::vector<EventHandler> handlers;
    if (it != listeners.end()) {
        handlers = it->second;
    } else {
        listeners.insert({ typeHash, handlers });
    }

    handlers.push_back(handler);
}

void oui::EventDispatcher::addSystemEventListener(std::string type, EventHandler handler) {
    auto typeHash = Event::createTypeHash(type);
    auto it = systemListeners.find(typeHash);

    std::vector<EventHandler> handlers;
    if (it != systemListeners.end()) {
        handlers = it->second;
    } else {
        systemListeners.insert({ typeHash, handlers });
    }

    handlers.push_back(handler);
}

void oui::EventDispatcher::dispatchEvent(ComponentEvent* event) {
    event->setTarget(target);

    triggerSystemListeners(event);

    if (!event->isPropagationStopped()) {
        triggerListeners(event);
    }

    Component* parent = (Component*) target->getParent();
    if (parent != NULL && event->bubbles) {
        if (event->eventClass == "mouse") {
            MouseEvent* mouseEvent = (MouseEvent*) event;
            event = mouseEvent->createBubbledEvent(mouseEvent);
        }
        parent->getEventDispatcher()->dispatchEvent(event);
    }
}

void oui::EventDispatcher::triggerListeners(ComponentEvent* event) {
    auto listenerIt = listeners.find(event->getTypeHash());

    if (listenerIt == listeners.end()) {
        return;
    }

    std::vector<EventHandler> handlers = listenerIt->second;
    for (std::size_t i = 0; i < handlers.size(); i++) {
        auto handler = handlers.at(i);
        handler(event);
    }
}

void oui::EventDispatcher::triggerSystemListeners(ComponentEvent* event) {
    auto listenerIt = systemListeners.find(event->getTypeHash());

    if (listenerIt == systemListeners.end()) {
        return;
    }

    std::vector<EventHandler> handlers = listenerIt->second;
    for (std::size_t i = 0; i < handlers.size(); i++) {
        auto handler = handlers.at(i);
        handler(event);
    }
}

