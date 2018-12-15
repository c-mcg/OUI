#include "OUI_Export.h"

namespace oui {

    struct OUI_API ParserState {
        int charIndex;
        int lineIndex;
        int lineCharIndex;
        int state;
        std::u16string token;

        ParserState();

    };

}