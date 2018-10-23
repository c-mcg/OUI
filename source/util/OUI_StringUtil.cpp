#include "util/OUI_StringUtil.h"

#include <locale> 
#include <codecvt>

#if _MSC_VER >= 1900

std::string convertUTF16toUTF8(std::u16string str) {
	std::string s;
	for (int i = 0; i < str.length(); i++) {
		s += (char) str.at(i);
	}
	return s;
}

std::u16string convertUTF8toUTF16(std::string string) {
	std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
	return std::u16string((char16_t*) convert.from_bytes(string).c_str());
}

#else

std::string convertUTF16toUTF8(std::u16string string) {
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return (std::string) convert.to_bytes(string);
}

std::u16string convertUTF8toUTF16(std::string string) {
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

int stringToInt(std::u16string str) {
	return std::stoi(convertUTF16toUTF8(str).c_str());
}

double stringToDouble(std::u16string str) {
	return std::stof(convertUTF16toUTF8(str).c_str());
}