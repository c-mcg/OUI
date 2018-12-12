
#include "osal/OSAL.h"
#include "oui/comp/OUI_ComponentLoader.h"
#include "oui/comp/OUI_DrawablePanel.h"
#include "oui/comp/OUI_Button.h"
#include "oui/comp/OUI_TextField.h"
#include "oui/comp/OUI_Menu.h"
#include "oui/comp/OUI_Panel.h"
#include "oui/comp/OUI_Label.h"
#include "oui/comp/OUI_ScrollPanel.h"

#include "util/OUI_StringUtil.h"

std::unordered_map<std::string, std::function<oui::Component*(std::string, std::string, std::vector<std::string>, std::vector<std::u16string>)>>  oui::ComponentLoader::tags;

bool oui::ComponentLoader::addTag(std::string tag, std::function < Component*(std::string, std::string, std::vector<std::string>, std::vector<std::u16string>)> createComponent) {
	auto it = tags.find(tag);
	if (it != tags.end()) {
		return false;
	}
	tags.insert({tag, createComponent});
	return true;
}

oui::ComponentLoader::ComponentLoader() {
	components = std::vector<oui::Component*>();
}

bool oui::ComponentLoader::loadComponents(std::u16string path) {
	std::ifstream stream(convertUTF16toUTF8(path));
	if(!stream.good()) {//File doesnt exist
		std::cout << "Could not find file: " << convertUTF16toUTF8(path.c_str()) << std::endl;
		return false;
	}
	std::string str =  std::string(std::istreambuf_iterator<char>(stream),
		std::istreambuf_iterator<char>());//Load file
	stream.close();

	oui::Container* currContainer = NULL;
	int state = 0;
	std::string tag = "";
	std::string attributeName = "";
	std::u16string attributeValue = u"";
	std::vector<std::string> attributeNames;
	std::vector<std::u16string> attributes;
	bool endTag = false;
	for(unsigned int i = 0; i < str.length(); i++) {
		char c = str.at(i);

		if(state == 0) {//looking for '<'
			endTag = false;
			if(c == '<') {
				state = 1;
			} else if(WHITE_SPACE.find(c) == -1) {
				//TODO error
			}
		} else if(state == 1) {//Looking for tag
			if(isalpha(c)) {
				tag = "";
				tag += c;
				attributeNames = std::vector<std::string>();
				attributes = std::vector<std::u16string>();
				state = 2;
			} else if(c == '/') {
				endTag = true;
			} else if(WHITE_SPACE.find(c) == -1) {
				//TODO error
			}
		} else if(state == 2) {//Building tag
			if(isalpha(c)) {
				tag += c;
			} else if(c == '"') {
				state = 4;
			} else if(c == '>' && endTag) {
				if(currContainer != NULL) {
					currContainer = (Container*) currContainer->getParent();
					state = 0;
				} else {
					//TODO error
				}
			} else if(WHITE_SPACE.find(c) != -1) {
				state = 3;
			} else {
				//TODO error
			}
		} else if(state == 3) {//Looking for attribute name
			if (isalpha(c)) {
				attributeName = c;
				state = 4;
			} else if(c == '>') {
				std::string name = "";
				std::string classes = "";
				for (int i = 0; i < attributeNames.size(); i++) {
					if (attributeNames.at(i) == "name") {
						name = convertUTF16toUTF8(attributes.at(i));
					}
					if (attributeNames.at(i) == "class") {
						classes = convertUTF16toUTF8(attributes.at(i));
					}
				}
				if (name == "") {
					std::cout << "THE NAME IS EMPTY?!?!?!?" << std::endl;
					//TODO error
				}
				Component* c = NULL;
				auto it = tags.find(tag);
				if (it != tags.end()) {
					c = it->second(name, classes, attributeNames, attributes);
				}

				if(c != NULL) {
					for (int i = 0; i < attributeNames.size(); i++) {
						if (attributeNames.at(i) != "name" && attributeNames.at(i) != "class") {
							c->parseAttribute(attributeNames.at(i), attributes.at(i));
						}
					}
					if(currContainer != NULL) {
						currContainer->addChild(c);
					} else {
						components.push_back(c);
					}
					if(c->isContainer()) {
						currContainer = (Container*) c;
					}
				}
				state = 0;
			} else if (WHITE_SPACE.find(c) != -1) {
				//TODO error
			}
		} else if(state == 4) {//Building attribute name
			if (isalpha(c) || c == '-') {
				attributeName += c;
			} else if (c == '=') {
				state = 5;
			} else if (WHITE_SPACE.find(c) != -1) {
				//TODO error
			}
		} else if(state == 5) {//Found '=' looking for '"'
			if (c == '"') {
				attributeValue = u"";
				state = 6;
			} else if (WHITE_SPACE.find(c) != -1) {
				//TODO error
			}
		} else if(state == 6) {//building attribute value
			if (c == '"') {
				attributeNames.push_back(attributeName);
				attributes.push_back(attributeValue);
				state = 3;
			} else {
				attributeValue += (char16_t) c;
			}
		} else if(state == 7) {
			
		}

	}

	return false;
}

bool oui::ComponentLoader::applyAttributes(std::u16string path) {
	OSAL::Sheet sheet = OSAL::parseSheet(path);
	for(unsigned int i = 0; i < components.size(); i++) {
		Component* c = components.at(i);
		//c->applyAttributes(sheet);
	}
	if(!attributes.isValid()) {
		attributes = sheet;
	} else {
		attributes = attributes.combine(sheet);
	}
	return false;
}

oui::Panel* oui::ComponentLoader::toPanel() {
	Panel* panel = new Panel("loadedpanel", "");
	panel->parseAttribute("size", u"100 100 0 0");
	panel->addOSALStyle(attributes);
	for(unsigned int i = 0; i < components.size(); i++) {
		panel->addChild(components.at(i));
	}
	return panel;
}