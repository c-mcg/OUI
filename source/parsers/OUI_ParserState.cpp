#include "parsers/OUI_ParserState.h"

oui::ParserState::ParserState() {
    charIndex = 0;
    lineIndex = 0;
    lineCharIndex = 0;
    state = 0;
    token = u"";
}