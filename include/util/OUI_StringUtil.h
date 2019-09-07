#ifndef OUI_STRING_UTIL_H
#define OUI_STRING_UTIL_H

#include "OUI_Export.h"
#include <string>

std::string OUI_API convertUTF16toUTF8(const std::u16string& string);

std::u16string OUI_API convertUTF8toUTF16(const std::string& string);

std::u16string OUI_API intToString(int i);

int OUI_API stringToInt(const std::u16string& str);

double OUI_API stringToDouble(const std::u16string& str);

std::u16string  OUI_API doubleToString(double d);

bool OUI_API stringStartsWith(std::u16string string, std::u16string starting);

bool OUI_API stringEndsWith(std::u16string string, std::u16string ending);
bool OUI_API stringEndsWith(std::string string, std::string ending);


std::string trimString(std::string s);
std::u16string trimString(std::u16string s);

#endif