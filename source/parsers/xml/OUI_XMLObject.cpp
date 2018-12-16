#include "parsers/xml/OUI_XMLObject.h"

oui::XMLObject::XMLObject(std::string tag, std::unordered_map<std::string, std::u16string> attributes) {
    this->tag = tag;
    this->attributes = attributes;
    this->children = std::vector<XMLObject>();
}


std::u16string oui::XMLObject::getAttribute(std::string attribute) {
    return attributes.at(attribute);
}

oui::XMLObject oui::XMLObject::getChild(int index) {
    return children.at(index);
}