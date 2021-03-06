
#include "osal/OSAL_Attribute.h"
#include <sstream>
#include <vector>
#include <iostream>
#include "osal/OSAL_Constants.h"
#include "util/OUI_StringUtil.h"

OSAL::Attribute::Attribute(const std::string& name, const std::u16string& value) :
    originalString{value}, name{name} {

    std::vector<std::u16string> tokens;
    std::u16string token = u"";
    for (unsigned int i = 0; i < value.length(); i++) {
        if (isspace(value.at(i)) && token != u"") {
            tokens.push_back(token);
            token = u"";
            continue;
        }
        token += value.at(i);
    }
    if (token != u"") {
        tokens.push_back(token);
    }

    bool quote = false;
    bool doubleQuote = false;
    std::u16string strLiteral;
    token = u"";
    for (unsigned int i = 0; i < tokens.size(); i++) {
        token = tokens.at(i);
        if(token.length() == 0) {
            continue;
        }
        if(token.at(0) == '\'' && !doubleQuote && !quote) {//Start quote
            quote = true;
            strLiteral = token.substr(1);
            if(token.at(token.length() - 1) == '\'') {//End quote
                strLiteral = token.substr(1, token.length() - 2);
                addValue(TYPE_STRING, strLiteral);
                quote = false;
            }
        } else if(token.at(token.length() - 1) == '\'' && quote) {//End quote
            quote = false;
            strLiteral += std::u16string(u" ").append(token.substr(0, token.length() - 1));
            addValue(TYPE_STRING, strLiteral);
        } else if(token.at(0) == '"' && !quote && !doubleQuote) {//Start double quote
            doubleQuote = true;
            strLiteral = token.substr(1);
            if(token.at(token.length() - 1) == '"') {//End double quote
                strLiteral = token.substr(1, token.length() - 2);
                addValue(TYPE_STRING, strLiteral);
                doubleQuote = false;
            }
        } else if(token.at(token.length() - 1) == '"' && doubleQuote) {//End double quote
            doubleQuote = false;
            strLiteral += std::u16string(u" ").append(token.substr(0, token.length() - 1));
            addValue(TYPE_STRING, strLiteral);
        } else if(quote || doubleQuote) {
            strLiteral += std::u16string(u" ").append(token);
        } else {
            if (isInteger(token)) {
                addValue(TYPE_INT, token);
            } else if (isDouble(token)) {
                addValue(TYPE_DOUBLE, token);
            } else if (equalsIgnoreCase(token, u"true") || equalsIgnoreCase(token, u"false")) {
                addValue(TYPE_BOOL, token);
            } else {
                addValue(TYPE_STRING, token);
            }
        }
    }

    if (this->value.size() == 0) {
        //TODO parse error
    }
}


OSAL::Attribute::Attribute(const std::string& name, const std::u16string& value, char type) :
    originalString{value}, name{name} {
    addValue(type, value);
}

std::string OSAL::Attribute::getName() {
    return name;
}

int OSAL::Attribute::getNumValues() {
    return (int) value.size();
}

char OSAL::Attribute::getType() {
    if (types.size() == 0 || types.size() > 1) {
        return TYPE_ARRAY;
    }

    return types[0];
}

std::u16string OSAL::Attribute::getAsString() {
    if (value.size() == 0) {
        return u"null";
    }
    return value[0];
}

int OSAL::Attribute::getAsInt() {
    std::u16string s = getAsString();
    if(s == u"null") {
        return 0;
    }
    return stringToInt(s);
}

double OSAL::Attribute::getAsDouble() {
    std::u16string s = getAsString();
    if(s == u"null") {
        return 0;
    }
    return stringToDouble(s);
}

bool OSAL::Attribute::getAsBool() {
    std::u16string s = getAsString();
    if(s == u"null") {
        return false;
    }
    return s.compare(u"true") == 0;
}

oui::Color OSAL::Attribute::getAsColor() {
    std::u16string s = getAsString();
    if(s == u"null") {
        return oui::Color::BLACK;
    }

    return oui::Color::fromString(s);
}

std::vector<OSAL::Attribute> OSAL::Attribute::getAsArray() {
    std::vector<Attribute> attributeArray;
    auto typeIt = types.begin();
    auto valueIt = value.begin();
    while(typeIt != types.end()) {
        attributeArray.push_back(Attribute("osal_attribute_array_val", *valueIt, *typeIt));
        typeIt++;
        valueIt++;
    }

    return attributeArray;
}

std::u16string OSAL::Attribute::getOriginalString() {
    return originalString;
}

bool OSAL::Attribute::isValid() {
    //return value.size() == 0 || value.at(0) != "null"; TODO
    return value.size() > 0 && value.at(0) != u"null";
}

void OSAL::Attribute::addValue(char type, std::u16string value) {

    // std::cout << convertUTF16toUTF8(trimString(value)).c_str() << std::endl;
    if (type == TYPE_STRING && stringStartsWith(trimString(value), u"#")) {
        types.push_back(TYPE_COLOR);
        this->value.push_back(value);
        return;
    }

    types.push_back(type);
    this->value.push_back(value);
}