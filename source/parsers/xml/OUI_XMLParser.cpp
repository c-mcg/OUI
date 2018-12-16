#include "parsers/xml/OUI_XMLParser.h"

oui::XMLParser::XMLParser(const std::u16string& source) {
    this->source = source;
    this->state = XMLParserState();
}

oui::XMLParser::XMLParser(const std::u16string& source, const XMLParserState& state) {
    this->source = source;
    this->state = state;
}

oui::XMLParserState oui::XMLParser::getState() {
    return state;
}
