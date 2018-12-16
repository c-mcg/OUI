#ifndef OUI_XMLPARSER_H
#define OUI_XMLPARSER_H

#include "OUI_Export.h"
#include <unordered_map>

#include "parsers/xml/OUI_XMLParserState.h"

namespace oui {

    class OUI_API XMLParser {

        private: std::u16string source;
        private: XMLParserState state;

        public: XMLParser(const std::u16string& source);
        public: XMLParser(const std::u16string& source, const XMLParserState& state);

        // start, pause, step, stop

        public: XMLParserState getState();

    };

}

#endif