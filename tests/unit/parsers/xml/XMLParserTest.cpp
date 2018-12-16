#include "TestUtil.h"

#include "parsers/xml/OUI_XMLParser.h"

TEST(XMLParser, itCanBeCreated)
{
    const oui::XMLParserState expectedState;
    oui::XMLParser parser(u"");
    EXPECT_EQ(parser.getState(), expectedState);
}