#ifndef OSAL_H
#define OSAL_H

/*
* This file is the main access point for the OSAL system
*/
#pragma once
#include "osal/OSAL_Sheet.h"
#include <fstream>
#include <streambuf>
#include "osal/OSAL_Constants.h"
#include <iostream>

#include "util/OUI_StringUtil.h"

namespace OSAL {

	static const int STATE_NONE = 0, STATE_START_SELECTORS = 1, STATE_BUILD_SELECTORS = 2, STATE_START_ATTRS = 3,
			STATE_ATTR_NAME = 4, STATE_ATTR_COLON = 5, STATE_START_ATTR_VAL = 6, STATE_BUILD_ATTR_VAL = 7;
	
	/*
		This function parses the specified file into a Sheet
		Sheet's name is "null" on error
	*/
	static Sheet parseSheet(std::u16string fileName) {
		
		std::wifstream stream(convertUTF16toUTF8(fileName));
		if(!stream.good()) {//File doesnt exist
			std::cout << "Could not find file: " << convertUTF16toUTF8(fileName.c_str()) << ".osal" << std::endl;
			return Sheet(u"null", {});
		}
		std::u16string str = std::u16string(std::istreambuf_iterator<wchar_t>(stream),
			std::istreambuf_iterator<wchar_t>());//Load file
		stream.close();

		

		std::vector<OSAL::Element> elements;//Elements in the sheet
		std::vector <OSAL::Attribute> attributes;//Attributes in the current elements
		std::vector<std::u16string> currElements;//The name of current elements being attributed (used when parsing attributes)
		std::string currAttribute;//The name of the current attribute (used when looking for attribute value)
		char state = STATE_NONE;//The current parsing state
		std::u16string currToken = u"";//The current token being parsed
		int lineNumber = 1;//The current line number being parsed
		wchar_t lastChar;
		char stringLiteral = 0;// '=1, "=2
		int type = -1;//The type of the current attribute
		std::vector<char> varTypes;//TODO remove type checking without messing up parsing
		int currChar = 0;
		for(unsigned int i = 0; i < str.length(); i++) {
			currChar++;
			wchar_t c = str.at(i); 
			if(c == '\n' || c == '\r') {
				lineNumber++;
				currChar = 0;
			}
			bool error = false;
			//std::cout << " char " << currChar << " '" << c << "' state=" << ((int) state) << " token=" << currToken << std::endl;
			switch(state) {

				case STATE_NONE://No state
					if(c == '.' || c == '#' || isalpha(c)) {//found letter
						currToken = c;
						state = STATE_START_SELECTORS;
					} else if(WHITE_SPACE.find(c) == -1) {//Invalid char
						error = true;
					}
					break;

				case STATE_START_SELECTORS://Building an element name
					if(isValidChar(c) || (currToken.size() > 1 && c == ':')) {//Add letter/number
						currToken += c;
					} else if(c == ',') {//Adding another element
						currElements.push_back(currToken);
						currToken = u"";
						state = STATE_NONE;
					} else if(c == '{') {//Done adding elements
						currElements.push_back(currToken);
						currToken = u"";
						state = STATE_START_ATTRS;
					} else if(WHITE_SPACE.find(c) != -1) {//continue if white space
						currElements.push_back(currToken);
						currToken = u"";
						state = STATE_BUILD_SELECTORS;
					} else {//Invalid char
						error = true;
					}
					break;

				case STATE_BUILD_SELECTORS://White space after element name
					if(c == '{') {//Done adding elements
						state = STATE_START_ATTRS;
					} else if(c == ',') {//Adding another element
						state = STATE_NONE;
					} else if(WHITE_SPACE.find(c) == -1) {//Invalid char
						error = true;
					}
					break;

				case STATE_START_ATTRS://Looking for attributes
					if(isalpha(c)) {//found letter
						currToken = c;
						state = STATE_ATTR_NAME;
					} else if(c == '}') {//Adding element(s) and continue search
						if(attributes.size() > 0) {
							for(unsigned int i = 0; i < currElements.size(); i++) {
								std::u16string curr = currElements.at(i);
								int colonPos = curr.find(u":");
								std::string element = convertUTF16toUTF8(colonPos == std::u16string::npos ? curr : curr.substr(0, colonPos));
								std::u16string profile = colonPos == std::string::npos ? u"default" : curr.substr(colonPos + 1, curr.size() - colonPos);
								//std::cout << " element: name=" << element << " profile=" << profile << std::endl;

								
								std::unordered_map<std::u16string, std::vector<Attribute>> map;
								map.insert({{profile, attributes}});
								Element e = Element(element, map);

								bool found = false;
								std::vector<OSAL::Element>::iterator it;
								for (it = elements.begin(); it != elements.end(); it++) {
									//std::cout << "compare " << e.getName() << " == " << it->getName() << " ? " << (it->getName() == e.getName()) << " textS=" << element << std::endl;
									if (it->getName() == e.getName()) {
										it->combine(e);
										found = true;
									}
								}
								if (!found) {
									elements.push_back(e);
								}
							}
						}
						attributes = {};
						currElements = {};
						currAttribute = "";
						state = STATE_NONE;
					} else if(WHITE_SPACE.find(c) == -1) {//Invalid char
						error = true;
					}
					break;

				case STATE_ATTR_NAME://Building an attribute name
					if(isValidChar(c)) {
						currToken += c;
					} else if(WHITE_SPACE.find(c) != -1) {//White space after attribute name
						state = STATE_ATTR_COLON;
					} else if(c == ':') {//Found ':'
						state = STATE_START_ATTR_VAL;
						currAttribute = convertUTF16toUTF8(currToken);
					} else {//Invalid char
						error = true;
					}
					break;

				case STATE_ATTR_COLON://Looking for ':' after attribute name
					if(c == ':') {//continue if white space
						state = STATE_START_ATTR_VAL;
						currAttribute = convertUTF16toUTF8(currToken);
						currToken = u"";
					} else if(WHITE_SPACE.find(c) == -1) {//Invalid char
						error = true;
					}
					break;

				case STATE_START_ATTR_VAL://Looking for attribute value
					if(NUMBERS.find(c) != -1) {//Start building value
						state = STATE_BUILD_ATTR_VAL;
						currToken = c;
						type = Attribute::INT;
					} else if(LETTERS.find(c) != -1) {//Start building value
						state = STATE_BUILD_ATTR_VAL;
						currToken = c;
						type = Attribute::STRING;
					} else if(c == '\'' || c == '"') {
						state = STATE_BUILD_ATTR_VAL;
						currToken = c;
						stringLiteral = c == '\'' ? 1 : 2;
						type = Attribute::STRING;
					} else if(WHITE_SPACE.find(c) == -1) {//Invalid char
						error = true;
					}
					//currToken = c;
					break;

				case STATE_BUILD_ATTR_VAL://Building attribute value
					if(stringLiteral != 0 && (stringLiteral == 2 || c != '\'')  && (stringLiteral == 1 || c != '"')) {
						currToken += c;
					} else if(c == '"' && stringLiteral != 1) {
						if(stringLiteral == 2) {
							stringLiteral = 0;
						} else if(WHITE_SPACE.find(lastChar) != -1) {
							stringLiteral = 2;
							type = Attribute::STRING;
						} else {
							error = true;
						}
						currToken += c;
					} else if(c == '\'' && stringLiteral != 2) {
						if(stringLiteral == 1) {
							stringLiteral = 0;
						} else if(WHITE_SPACE.find(lastChar) != -1) {
							stringLiteral = 1;
							type = Attribute::STRING;
						} else {
							error = true;
						}
						currToken += c;
					} else if(NUMBERS.find(c) != -1 || (c == '.' && (type == -1 || type == Attribute::INT || type == Attribute::DOUBLE)))  {//Add character
						if(c == '.' && type == Attribute::INT) {
							type = Attribute::DOUBLE;
						}
						if (type == -1) {
							type = Attribute::INT;
						}
						currToken += c;
						lastChar = c;
					} else if(isValidChar(c) || c == ' ') {//Add character
						currToken += c;
						lastChar = c;
						if (c == ' ' && type != -1) {
							varTypes.push_back((char) type);
							type = -1;//This should make it so multiple types are allowed
						}
					} else if(c != '\n' && WHITE_SPACE.find(c) != -1) {
						currToken += c;
						lastChar = c;
					} else if(c == ';') {//Time to look for another attribute
						if (type != -1) {
							varTypes.push_back((char) type);
							type = -1;//This should make it so multiple types are allowed
						}
						attributes.push_back(Attribute(currAttribute, currToken.c_str()));
						varTypes = std::vector<char>();
						state = STATE_START_ATTRS;
					} else if(c != '\n') {//Invalid char
						error = true;
					}
					break;

			}
			if(error) {//Error
				std::cout << "Error in sheet " << convertUTF16toUTF8(fileName).c_str() << " on line " << lineNumber << " char " << currChar << " '" << c << "' state=" << ((int)state) << " token=" << currToken.c_str() << std::endl;
				return Sheet(u"null", {});
			}
		}

		//Remove path and extension from file name
		int fpos = fileName.find('/');
		int lpos = fileName.find('.');
		std::u16string name;
		if(fpos != -1) {
			if(lpos != -1) {
				name = fileName.substr(fpos, lpos);
			} else {
				name = fileName.substr(fpos);
			}
		} else {
			if(lpos != -1) {
				name = fileName.substr(0, lpos);
			} else {
				name = fileName;
			}
		}

		return Sheet(name, elements);
	}

}

#endif // !OSAL_H