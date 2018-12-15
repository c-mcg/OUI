#ifndef OUI_PARSERSTATE_H
#define OUI_PARSERSTATE_H

#include "OUI_Export.h"

namespace oui {

    struct OUI_API ParserState {
        int charIndex;
        int lineIndex;
        int lineCharIndex;
        int state;
        std::u16string token;

        ParserState();

        public: bool operator== (const ParserState& c1) const;
        public: bool operator!= (const ParserState& c1) const;
    };
}

#endif