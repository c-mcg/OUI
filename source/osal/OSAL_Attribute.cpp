
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
        if (value.at(i) == u' ') {
            tokens.push_back(token);
            token = u"";
            continue;
        }
        token += value.at(i);
    }
    if (token.length() > 0) {
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
                types.push_back(TYPE_STRING);
                this->value.push_back(strLiteral);
                quote = false;
            }
        } else if(token.at(token.length() - 1) == '\'' && quote) {//End quote
            quote = false;
            strLiteral += std::u16string(u" ").append(token.substr(0, token.length() - 1));
            types.push_back(TYPE_STRING);
            this->value.push_back(strLiteral);
        } else if(token.at(0) == '"' && !quote && !doubleQuote) {//Start double quote
            doubleQuote = true;
            strLiteral = token.substr(1);
            if(token.at(token.length() - 1) == '"') {//End double quote
                strLiteral = token.substr(1, token.length() - 2);
                types.push_back(TYPE_STRING);
                this->value.push_back(strLiteral);
                doubleQuote = false;
            }
        } else if(token.at(token.length() - 1) == '"' && doubleQuote) {//End double quote
            doubleQuote = false;
            strLiteral += std::u16string(u" ").append(token.substr(0, token.length() - 1));
            types.push_back(TYPE_STRING);
            this->value.push_back(strLiteral);
        } else if(quote || doubleQuote) {
            strLiteral += std::u16string(u" ").append(token);
        } else {
            if (isInteger(token)) {
                types.push_back(TYPE_INT);
            } else if (isDouble(token)) {
                types.push_back(TYPE_DOUBLE);
            } else if (equalsIgnoreCase(token, u"true") || equalsIgnoreCase(token, u"false")) {
                //std::cout << "parsed bool: " << convertUTF16toUTF8(token).c_str() << std::endl;
                types.push_back(TYPE_BOOL);
            } else {
                //std::cout << "Defaulted to string for value " << convertUTF16toUTF8(token).c_str() << std::endl;
                types.push_back(TYPE_STRING);
            }
            this->value.push_back(token);
        }
    }

    if (this->value.size() == 0) {
        //TODO parse error
    }
}

std::string OSAL::Attribute::getName() {
    return name;
}

int OSAL::Attribute::getNumValues() {
    return (int) value.size();
}

char OSAL::Attribute::getType(int index) {
    if(index < 0 || index >= value.size()) {
        return -1;
    }
    return types[index];
}

std::u16string OSAL::Attribute::getAsString(int index) {
    if(index < 0 || index >= value.size()) {
        return u"null";
    }
    return value[index];
}

int OSAL::Attribute::getAsInt(int index) {
    std::u16string s = getAsString(index);
    if(s == u"null") {
        return 0;
    }
    return stringToInt(s);
}

double OSAL::Attribute::getAsDouble(int index) {
    std::u16string s = getAsString(index);
    if(s == u"null") {
        return 0;
    }
    return stringToDouble(s);
}

bool OSAL::Attribute::getAsBool(int index) {
    std::u16string s = getAsString(index);
    if(s == u"null") {
        return false;
    }
    return getAsString(index).compare(u"true") == 0;
}

std::u16string OSAL::Attribute::getOriginalString() {
    return originalString;
}

bool OSAL::Attribute::isValid() {
    //return value.size() == 0 || value.at(0) != "null"; TODO
    return value.size() > 0 && value.at(0) != u"null";
}