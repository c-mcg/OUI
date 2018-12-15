#include "gtest/gtest.h"

#include "parsers/xml/OUI_XMLParser.h"
// #include "util/OUI_StringUtil.h"

TEST(XMLParser, itCanBeCreated)
{
    const oui::ParserState expectedState;
    oui::XMLParser parser;
    EXPECT_EQ(parser.getState(), expectedState);
    EXPECT_EQ(parser.getState().state, oui::XMLParserState::NONE);
}