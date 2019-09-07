#include "util/OUI_StringUtil.h"

#include <locale> 
#include <codecvt>
#include <algorithm>
#include <functional>
#include <cctype>

#if _MSC_VER >= 1900

std::string convertUTF16toUTF8(const std::u16string& str) {
    std::string s;
    for (int i = 0; i < str.length(); i++) {
        s += (char) str.at(i);
    }
    return s;
}

std::u16string convertUTF8toUTF16(const std::string& string) {
    std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
    return std::u16string((char16_t*) convert.from_bytes(string).c_str());
}

#else

std::string convertUTF16toUTF8(const std::u16string& string) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return (std::string) convert.to_bytes(string);
}

std::u16string convertUTF8toUTF16(const std::string& string) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return (std::u16string) convert.from_bytes(string);
}

#endif



std::u16string intToString(int i) {
    return convertUTF8toUTF16(std::to_string(i));
}

std::u16string doubleToString(double d) {
    return convertUTF8toUTF16(std::to_string(d));
}

#include <iostream>
int stringToInt(const std::u16string& str) {
    return std::stoi(convertUTF16toUTF8(str).c_str());
}

double stringToDouble(const std::u16string& str) {
    return std::stof(convertUTF16toUTF8(str).c_str());
}

bool stringStartsWith(std::u16string string, std::u16string starting) {
    if (string.length() < starting.length()) {
        return false;
    }
    
    auto stringIt = string.begin();
    auto startingIt = starting.begin();

    int charactersChecked = 0;
    while (charactersChecked < starting.length()) {
        if (*stringIt != *startingIt) {
            return false;
        }
        charactersChecked++;
        stringIt++;
        startingIt++;
    }

    return true;
}


bool stringEndsWith(std::u16string string, std::u16string ending) {
    if (string.length() < ending.length()) {
        return false;
    }
    
    auto stringIt = string.end() - 1;
    auto endingIt = ending.end() - 1;

    int charactersChecked = 0;
    while (charactersChecked < ending.length()) {
        if (*stringIt != *endingIt) {
            return false;
        }
        charactersChecked++;
        stringIt--;
        endingIt--;
    }

    return true;
}

bool stringEndsWith(std::string string, std::string ending) {
    if (string.length() < ending.length()) {
        return false;
    }
    
    auto stringIt = string.end() - 1;
    auto endingIt = ending.end() - 1;

    int charactersChecked = 0;
    while (charactersChecked < ending.length()) {
        if (*stringIt != *endingIt) {
            return false;
        }
        charactersChecked++;
        stringIt--;
        endingIt--;
    }

    return true;
}

std::string trimString(std::string s) {
    // https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    // trim from start (in place)
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));

    // trim from end (in place)
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());

    return s;
}

std::u16string trimString(std::u16string s) {
    // https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    // trim from start (in place)
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));

    // trim from end (in place)
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());

    return s;
}