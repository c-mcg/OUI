#include "gtest/gtest.h"

#include "parsers/OUI_ParserState.h"
#include "util/OUI_StringUtil.h"

TEST(ParserState, itCanBeCreated)
{
    oui::ParserState state;
    EXPECT_EQ(state.charIndex, 0);
    EXPECT_EQ(state.lineIndex, 0);
    EXPECT_EQ(state.lineCharIndex, 0);
    EXPECT_EQ(state.state, 0);
    EXPECT_STREQ(convertUTF16toUTF8(state.token).c_str(), "");
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

TEST(ParserState, itBeUnequal_state)
{
    oui::ParserState state1;
    state1.state = 1;
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