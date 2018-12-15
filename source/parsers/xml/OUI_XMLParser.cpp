#include "parsers/xml/OUI_XMLParser.h"

oui::XMLParser::XMLParser() {
    this->state = ParserState();
}

oui::XMLParser::XMLParser(const ParserState& state) {
    this->state = state;
}

oui::ParserState oui::XMLParser::getState() {
    return state;
}
