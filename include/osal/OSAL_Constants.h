#ifndef OUI_OSAL_CONSTANTS_H
#define OUI_OSAL_CONSTANTS_H

/*
* This file holds string constants and determines if characters are alphanumeric
* Should be named string tools and should be refactored out
*/
#include <cctype>
#include <algorithm>

#include <string>
#include <locale> 
#include <codecvt>
#include <ctype.h>

#include "util/OUI_StringUtil.h"

static bool isValidChar(wchar_t c);

static bool isInteger(const std::u16string& str) {
	for (int i = 0; i < str.size(); i++) {
		if (i == 0 && str.at(i) == '-') {
			continue;
		}
		if (!isdigit(str.at(i))) {
			return false;
		}
	}
	return true;
}

static bool isDouble(const std::u16string& str) {
	bool foundDecimal = false;
	for (int i = 0; i < str.size(); i++) {
		if (i == 0 && str.at(i) == '-') {
			continue;
		}
		if (!isdigit(str.at(i))) {
			if (str.at(i) == '.' && !foundDecimal) {
				foundDecimal = true;
			} else {
				return false;
			}
		}
	}
	return true;
}

static bool equalsIgnoreCase(const std::u16string& s, const std::u16string& s1) {
	if (s.size() != s1.size()) {
		return false;
	}
	for (int i = 0; i < s.size(); i++) {
		if (std::tolower(s.at(i)) != std::tolower(s1.at(i))) {
			return false;
		}
	}
	return true;
}

/*
* Determines if the string contains only alphnumeric chars
*/
static bool isValidString(const std::u16string& str) {
	for(unsigned int i = 0; i < str.length(); i++) {
		if(!isValidChar(str.at(i))) {
			return false;
		}
	}
	return true;
}

/*
* Determines if the char is alphanumeric
*/
static bool isValidChar(wchar_t c) {
	return isalpha(c) || isdigit(c) || c == '-';
}

#endif
