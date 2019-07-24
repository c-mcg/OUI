#include <functional>

#include "event/OUI_Event.h"
#include <iostream>

bool oui::Event::compareTypes(const Event& event1, const Event& event2) {
    return event1.type == event2.type;
}

oui::Event::Event(std::string eventClass, std::string type)
    : eventClass{eventClass}, type{type}, defaultPrevented{false} {}

bool oui::Event::compareType(const Event& event) {
    return this->type == event.type;
}

bool oui::Event::compareType(std::string type) {
    return this->type == type;
}

void oui::Event::preventDefault() {
    defaultPrevented = true;
}

bool oui::Event::isDefaultPrevented() {
    return defaultPrevented;
}