#ifndef OUI_STRING_UTIL_H
#define OUI_STRING_UTIL_H

#include "OUI_Export.h"
#include <string>

std::string OUI_API convertUTF16toUTF8(std::u16string string);

std::u16string OUI_API convertUTF8toUTF16(std::string string);

std::u16string OUI_API intToString(int i);

int OUI_API stringToInt(std::u16string str);

double OUI_API stringToDouble(std::u16string str);

#endif