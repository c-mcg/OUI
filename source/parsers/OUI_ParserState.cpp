#include "parsers/OUI_ParserState.h"

oui::ParserState::ParserState() {
    charIndex = 0;
    lineIndex = 0;
    lineCharIndex = 0;
    name = "";
    token = u"";
}

bool oui::ParserState::operator== (const ParserState& other) const {
    return other.charIndex == charIndex &&
        other.lineIndex == lineIndex &&
        other.lineCharIndex == lineCharIndex &&
        other.name == name &&
        other.token == token;
}

bool oui::ParserState::operator!= (const ParserState& other) const {
    return !((*this) == other);
}