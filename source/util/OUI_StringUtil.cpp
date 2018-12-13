#include "util/OUI_StringUtil.h"

#include <locale> 
#include <codecvt>
#include <algorithm>
#include <functional>

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

// std::u16string::const_iterator findInString(const std::u16string& string, const char toFind) {
// 	const char16_t toFind16 = (char16_t) toFind;
// 	return std::search(string.cbegin(), string.cend(), std::boyer_moore_searcher(&toFind16, &toFind16)); 
// }

// std::u16string::const_iterator findInString(const std::u16string& string, const char16_t toFind) {
// 	return std::search(string.cbegin(), string.cend(), std::boyer_moore_searcher(&toFind, &toFind)); 
// }

// std::u16string::const_iterator findInString(const std::u16string& string, const std::string& toFind) {
// 	const std::u16string toFind16 = convertUTF8toUTF16(toFind);
// 	return std::search(string.cbegin(), string.cend(), std::boyer_moore_searcher(toFind16.begin(), toFind16.end())); 
// }

// std::u16string::const_iterator findInString(const std::u16string& string, const std::u16string& toFind) {
// 	return std::search(string.cbegin(), string.cend(), std::boyer_moore_searcher(toFind.begin(), toFind.end())); 
// }