#include <functional>

#include "event/OUI_Event.h"
#include <iostream>

std::hash<std::string> oui::Event::hasher = std::hash<std::string>();

std::size_t oui::Event::createTypeHash(std::string type) {
    return hasher(type);
}

bool oui::Event::compareTypes(const Event& event1, const Event& event2) {
    return event1.typeHash == event2.typeHash;
}

oui::Event::Event(std::string eventClass, std::string type, std::size_t typeHash)
    : eventClass{eventClass}, type{type}, defaultPrevented{false} {

    // Create a `typeHash` if one isn't given
    if (typeHash == 0) {
        typeHash = Event::createTypeHash(type);
    }

    this->typeHash = (const std::size_t) typeHash;
}

bool oui::Event::compareType(std::size_t typeHash) {
    std::cout << "Comparing event type hash " << this->typeHash << " " << typeHash << std::endl;
    return this->typeHash == typeHash;
}

bool oui::Event::compareType(const Event& event) {
    return this->typeHash == event.typeHash;
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

size_t oui::Event::getTypeHash() {
    return typeHash;
}