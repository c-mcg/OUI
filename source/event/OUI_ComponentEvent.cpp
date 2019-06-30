#include <functional>

#include "event/OUI_ComponentEvent.h"
#include "components/OUI_Component.h"

oui::ComponentEvent::ComponentEvent(Component* originalTarget, std::string type, std::size_t typeHash,  bool bubbles)
    : Event2(type, typeHash), originalTarget{originalTarget}, target{originalTarget}, bubbles{bubbles}, propagationStopped{false} {
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