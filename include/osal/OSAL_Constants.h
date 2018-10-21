/*
* This file holds string constants and determines if characters are alphanumeric
* Should be named string tools and should be refactored out
*/
#pragma once
#include <cctype>
#include <algorithm>

#include <string>
#include <locale> 
#include <codecvt>

//String of letters
const std::u16string LETTERS = u"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//String of numbers
const std::u16string NUMBERS = u"1234567890";
//String of white space chars
const std::u16string WHITE_SPACE = u" \t\r\n";

static bool isValidChar(wchar_t c);

static bool isInteger(std::u16string str) {
	for (int i = 0; i < str.size(); i++) {
		if (i == 0 && str.at(i) == '-') {
			continue;
		}
		if (NUMBERS.find(str.at(i)) == -1) {
			return false;
		}
	}
	return true;
}

static bool isDouble(std::u16string str) {
	bool foundDecimal = false;
	for (int i = 0; i < str.size(); i++) {
		if (i == 0 && str.at(i) == '-') {
			continue;
		}
		if (NUMBERS.find(str.at(i)) == -1) {
			if (str.at(i) == '.' && !foundDecimal) {
				foundDecimal = true;
			} else {
				return false;
			}
		}
	}
	return true;
}

static bool equalsIgnoreCase(std::u16string s, std::u16string s1) {
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
static bool isValidString(std::u16string str) {
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

