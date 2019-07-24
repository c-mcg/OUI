#include <functional>

#include "event/OUI_ComponentEvent.h"
#include "components/OUI_Component.h"

oui::ComponentEvent::ComponentEvent(Component* originalTarget, std::string eventClass, std::string type, bool bubbles)
    : Event(eventClass, type), originalTarget{originalTarget}, target{originalTarget}, bubbles{bubbles}, propagationStopped{false} {
}

void oui::ComponentEvent::stopPropagation() {
    propagationStopped = true;
}

bool oui::ComponentEvent::isPropagationStopped() {
    return propagationStopped;
}

oui::Component* oui::ComponentEvent::getTarget() {
    return target;
}

void oui::ComponentEvent::setTarget(Component* target) {
    this->target = target;
}