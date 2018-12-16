#include "TestUtil.h"

#include "parsers/OUI_ParserState.h"

TEST(ParserState, itCanBeCreated)
{
    oui::ParserState state;
    EXPECT_EQ(state.charIndex, 0);
    EXPECT_EQ(state.lineIndex, 0);
    EXPECT_EQ(state.lineCharIndex, 0);
    EXPECT_EQ(state.name, "");
    EXPECT_STR16_EQUAL(state.token, u"");
}

TEST(ParserState, itCanBeEqual)
{
    oui::ParserState state1;
    oui::ParserState state2;
    EXPECT_EQ(state1, state2);
}

TEST(ParserState, itBeUnequal_charIndex)
{
    oui::ParserState state1;
    state1.charIndex = 1;
    oui::ParserState state2;
    EXPECT_NE(state1, state2);
}

TEST(ParserState, itBeUnequal_lineIndex)
{
    oui::ParserState state1;
    state1.lineIndex = 1;
    oui::ParserState state2;
    EXPECT_NE(state1, state2);
}

TEST(ParserState, itBeUnequal_lineCharIndex)
{
    oui::ParserState state1;
    state1.lineCharIndex = 1;
    oui::ParserState state2;
    EXPECT_NE(state1, state2);
}

TEST(ParserState, itBeUnequal_name)
{
    oui::ParserState state1;
    state1.name = "1";
    oui::ParserState state2;
    EXPECT_NE(state1, state2);
}

TEST(ParserState, itBeUnequal_token)
{
    oui::ParserState state1;
    state1.token = u"1";
    oui::ParserState state2;
    EXPECT_NE(state1, state2);
}