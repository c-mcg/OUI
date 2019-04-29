
#include "osal/OSAL_Sheet.h"

#include <iostream>//TODO

OSAL::Sheet::Sheet() :
    name{u"null"} {
}

OSAL::Sheet::Sheet(const std::u16string& name, const std::vector<Element>& elements) :
    name{name}, elements{std::vector<Element>(elements)} {
}

OSAL::Sheet OSAL::Sheet::combine(Sheet sheet) {
    std::vector<Element> ele = std::vector<Element>(elements);
    ele.insert(ele.end(), sheet.elements.begin(), sheet.elements.end());
    return Sheet(u"custom", ele);
}

bool OSAL::Sheet::isValid() {
    return name != u"null";
}

std::u16string OSAL::Sheet::getName() {
    return name;
}

OSAL::Element OSAL::Sheet::getElement(Element::Type type, const std::string& name) {
    if(name.find(" ") != std::string::npos) {
        return Element();
    }
    for(unsigned int i = 0; i < elements.size(); i++) {
        if(elements.at(i).getType() == type && elements.at(i).getName().compare(name) == 0) {
            return elements.at(i);
        }
    }
    return Element();
}

OSAL::Element OSAL::Sheet::getElement(const std::string& name) {
    if(name.find(" ") != std::string::npos) {
        return Element();
    }
    char type = name.at(0) == '#' ? 2 : (name.at(0) == '.' ? 1 : 0);
    return getElement((Element::Type) type, name.substr(type == 0 ? 0 : 1));
}

std::vector<OSAL::Element> OSAL::Sheet::getElements() {
    return elements;
}