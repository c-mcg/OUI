#include <functional>

#include "event/OUI_Event2.h"

std::size_t oui::Event2::createTypeHash(std::string type) {
    return hasher(type);
}

bool oui::Event2::compareTypes(const Event2& event1, const Event2& event2) {
    return event1.typeHash == event2.typeHash;
}

oui::Event2::Event2(std::string type, std::size_t typeHash)
    : type{type}, defaultPrevented{false} {

    // Create a `typeHash` if one isn't given
    if (typeHash == 0) {
        typeHash = Event2::createTypeHash(type);
    }

    this->typeHash = (const std::size_t) typeHash;
}

bool oui::Event2::compareType(std::size_t typeHash) {
    return this->typeHash == typeHash;
}

bool oui::Event2::compareType(const Event2& event) {
    return this->typeHash == event.typeHash;
}

bool oui::Event2::compareType(std::string type) {
    return this->type == type;
}

void oui::Event2::preventDefault() {
    defaultPrevented = true;
}

bool oui::Event2::isDefaultPrevented() {
    return defaultPrevented;
}