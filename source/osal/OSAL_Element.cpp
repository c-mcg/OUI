
#include "osal/OSAL_Element.h"

OSAL::Element::Element() : Element(TAG, "null", std::unordered_map<std::u16string, std::vector<Attribute>>()) {
}

OSAL::Element::Element(Type type, std::string name, std::unordered_map<std::u16string, std::vector<Attribute>> attributes) {
	setValues(type, name, attributes);
}

OSAL::Element::Element(std::string name, std::unordered_map<std::u16string, std::vector<Attribute>> attributes) {
	char type = name.at(0) == '#' ? 2 : (name.at(0) == '.' ? 1 : 0);
	setValues((Type) type, name.substr(type == 0 ? 0 : 1), attributes);
}

void OSAL::Element::setValues(Type type, std::string name, std::unordered_map<std::u16string, std::vector<Attribute>> attributes) {
	this->type = type;
	this->name = name;
	this->attributes = attributes;
}

void OSAL::Element::combine(Element e) {
	for (auto it = e.attributes.begin(); it != e.attributes.end(); it++) {
		auto it2 = attributes.find(it->first);
		if (it2 != attributes.end()) {
			it2->second.insert(it2->second.end(), it->second.begin(), it->second.end());
		} else {
			attributes.insert({it->first, it->second});
		}
	}
}

OSAL::Element::Type OSAL::Element::getType() {
	return type;
}

std::string OSAL::Element::getName() {
	return name;
}

std::vector<OSAL::Attribute> OSAL::Element::getAttributes(std::u16string profile) {
	std::unordered_map<std::u16string, std::vector<Attribute>>::iterator it = attributes.find(profile);
	if (it != attributes.end()) {
		return it->second;
	}
	//TODO error or something
	return std::vector<OSAL::Attribute>();
}

OSAL::Attribute OSAL::Element::getAttribute(std::string name, std::u16string profile) {
	std::unordered_map<std::u16string, std::vector<Attribute>>::iterator it = attributes.find(profile);
	if (it != attributes.end()) {
		std::vector<Attribute> attrs = it->second;
		for(unsigned int i = 0; i < attrs.size(); i++) {
			if(attrs.at(i).getName().compare(name) == 0) {
				return attrs.at(i);
			}
		}
		return Attribute(name, u"null");
	}
	
	//TODO error
	return Attribute(name, u"null");
}

bool OSAL::Element::isValid() {
	return name != "null";
}

std::unordered_map<std::u16string, std::vector<OSAL::Attribute>> OSAL::Element::getAttributes() {
	return attributes;
}