#ifndef OUI_XMLPARSER_H
#define OUI_XMLPARSER_H

#include "OUI_Export.h"

#include "parsers/OUI_ParserState.h"

namespace oui {

    enum OUI_API XMLParserState {
        NONE,
        
    };

    class OUI_API XMLParser {
        private: ParserState state;

        public: XMLParser();
        public: XMLParser(const ParserState& state);

        public: ParserState getState();

    };

}

#endif