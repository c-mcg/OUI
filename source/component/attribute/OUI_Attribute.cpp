#include "component/attribute/OUI_Attribute.h"
#include "osal/OSAL_Attribute.h"
#include "util/OUI_StringUtil.h"
#include "exception/OUI_ArgumentException.h"

#include <iostream>

oui::Attribute::~Attribute() {
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
    } else if (a.type == OSAL::TYPE_COLOR) {
        colorVal = a.colorVal;
    } else if (a.type == OSAL::TYPE_ARRAY) {
        arrayVal = a.arrayVal;
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

oui::Attribute::Attribute(const char16_t* value) :
    Attribute(std::u16string(value)) {

}

oui::Attribute::Attribute(const std::u16string value) :
    type{OSAL::TYPE_STRING}, stringVal{value} {
}

oui::Attribute::Attribute(bool value) {
    type = OSAL::TYPE_BOOL;
    boolVal = value;
}

oui::Attribute::Attribute(double value) {
    type = OSAL::TYPE_DOUBLE;
    doubleVal = value;
}

oui::Attribute::Attribute(Color value) {
    type = OSAL::TYPE_COLOR;
    colorVal = value;
}

oui::Attribute::Attribute(std::vector<Attribute> value) {
    type = OSAL::TYPE_ARRAY;
    arrayVal = value;
}

oui::Attribute::Attribute(OSAL::Attribute value) {
    this->type = value.getType();
    switch (this->type) {
        case OSAL::TYPE_INT:
            intVal = value.getAsInt();
            break;
        case OSAL::TYPE_STRING:
            stringVal = value.getAsString();
            break;
        case OSAL::TYPE_DOUBLE:
            doubleVal = value.getAsDouble();
            break;
        case OSAL::TYPE_BOOL:
            boolVal = value.getAsBool();
            break;
        case OSAL::TYPE_COLOR:
            colorVal = value.getAsColor();
            break;
        case OSAL::TYPE_ARRAY:
            std::vector<OSAL::Attribute> osalArray = value.getAsArray();
            std::vector<oui::Attribute> ouiArray;
            for (auto it = osalArray.begin(); it != osalArray.end(); it++) {
                ouiArray.push_back(oui::Attribute(*it));
            }
            arrayVal = ouiArray;
            break;
    }
}

std::u16string oui::Attribute::asString() {
    if (type != OSAL::TYPE_STRING) {
        throw ArgumentException(
            "Attribute",
            "asString",
            "Tried to get a string value from attribute that is not a string",
            "Either the attribute is incorrectly set, or you should be using a getter for a different type"
        );
    }

    return stringVal;
}

int oui::Attribute::asInt() {
    if (type != OSAL::TYPE_INT) {
        throw ArgumentException(
            "Attribute",
            "asInt",
            "Tried to get an int value from attribute that is not an int",
            "Either the attribute is incorrectly set, or you should be using a getter for a different type"
        );
    }

    return intVal;
}

double oui::Attribute::asDouble() {
    if (type != OSAL::TYPE_DOUBLE) {
        throw ArgumentException(
            "Attribute",
            "asDouble",
            "Tried to get a double value from attribute that is not a double",
            "Either the attribute is incorrectly set, or you should be using a getter for a different type"
        );
    }

    return doubleVal;
}

bool oui::Attribute::asBool() {
    if (type != OSAL::TYPE_BOOL) {
        throw ArgumentException(
            "Attribute",
            "asBool",
            "Tried to get a bool value from attribute that is not a bool",
            "Either the attribute is incorrectly set, or you should be using a getter for a different type"
        );
    }

    return boolVal;
}

oui::Color oui::Attribute::asColor() {
    if (type != OSAL::TYPE_COLOR) {
        throw ArgumentException(
            "Attribute",
            "asColor",
            "Tried to get a color value from attribute that is not a color",
            "Either the attribute is incorrectly set, or you should be using a getter for a different type"
        );
    }

    return colorVal;
}

std::vector<oui::Attribute> oui::Attribute::asArray() {
    if (type != OSAL::TYPE_ARRAY) {
        return std::vector<Attribute>{*this};
    }

    return arrayVal;
}

std::vector<std::u16string> oui::Attribute::asStringArray() {
    auto attributeArray = asArray();
    std::vector<std::u16string> stringArray;
    for(auto it = attributeArray.begin(); it != attributeArray.end(); it++) {
        stringArray.push_back(it->asString());
    }

    return stringArray;
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
        case OSAL::TYPE_COLOR:
            return convertUTF8toUTF16(colorVal.toString());
        case OSAL::TYPE_ARRAY:
            std::u16string arrayString = u"[";
            for (auto it = arrayVal.begin(); it != arrayVal.end(); it++) {
                arrayString += (*it).toString();
                if (it != arrayVal.end() - 1) {
                    arrayString == u", ";
                }
            }
            arrayString += u"]";
            return arrayString;
    }
    return stringVal;
}

bool oui::Attribute::equals(Attribute other) {

    //Return false if types are different
    if (type != other.type) {
        return false;
    }

    //Compare member values
    return intVal == other.intVal && stringVal == other.stringVal &&
        boolVal == other.boolVal && doubleVal == other.doubleVal &&
        colorVal.getARGB() == other.colorVal.getARGB();
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
        case OSAL::TYPE_COLOR:
            return new Attribute(colorVal);
            break;
        case OSAL::TYPE_ARRAY:
            return new Attribute(arrayVal);
            break;
    }

    return NULL;// TODO (error?) This should be a breakpoint as it should never happen
}