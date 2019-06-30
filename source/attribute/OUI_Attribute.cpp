#include "attribute/OUI_Attribute.h"
#include "osal/OSAL_Attribute.h"
#include "util/OUI_StringUtil.h"

#include <iostream>

oui::Attribute::~Attribute() {
    //TODO!!!!!
}

oui::Attribute::Attribute() {

}

oui::Attribute::Attribute(const Attribute& a) {
    type = a.type;
    if (a.type == OSAL::TYPE_INT) {
        intVal = a.intVal;
    } else if (a.type == OSAL::TYPE_STRING) {
        stringVal = a.stringVal;
    } else if (a.type == OSAL::TYPE_BOOL) {
        boolVal = a.boolVal;
    } else if (a.type == OSAL::TYPE_DOUBLE) {
        doubleVal = a.doubleVal;
    }
}

oui::Attribute::Attribute(int value) {
    type = OSAL::TYPE_INT;
    intVal = value;
}

oui::Attribute::Attribute(const std::string value) : 
    Attribute(convertUTF8toUTF16(value)) {
}

oui::Attribute::Attribute(const char* value) : 
    Attribute(std::string(value)) {
}

oui::Attribute::Attribute(const std::u16string value) :
    type{OSAL::TYPE_STRING}, stringVal{value} {
}

oui::Attribute::Attribute(const char16_t* value) :
    Attribute(std::u16string(value)) {

}

oui::Attribute::Attribute(bool value) {
    type = OSAL::TYPE_BOOL;
    boolVal = value;
}

oui::Attribute::Attribute(double value) {
    type = OSAL::TYPE_DOUBLE;
    doubleVal = value;
}

std::u16string oui::Attribute::toString() {
    switch (type) {
        case OSAL::TYPE_INT:
            return intToString(intVal);
        case OSAL::TYPE_STRING:
            return stringVal;
        case OSAL::TYPE_BOOL:
            return boolVal ? u"true" : u"false";
        case OSAL::TYPE_DOUBLE:
            return doubleToString(doubleVal);
            break;
    }
    return stringVal;
}

bool oui::Attribute::equals(Attribute other) {

    //Return false if types are different
    if (type != other.type) {
        return false;
    }

    //Compare member values
    return intVal == other.intVal && stringVal == other.stringVal
        && boolVal == other.boolVal && doubleVal == other.doubleVal;
}

oui::Attribute* oui::Attribute::clone() {
    switch (type) {
        case OSAL::TYPE_INT:
            return new Attribute(intVal);
            break;
        case OSAL::TYPE_STRING:
            return new Attribute(stringVal);
            break;
        case OSAL::TYPE_BOOL:
            return new Attribute(boolVal);
            break;
        case OSAL::TYPE_DOUBLE:
            return new Attribute(doubleVal);
            break;
    }

    return NULL;//This should be a breakpoint as it should never happen
}