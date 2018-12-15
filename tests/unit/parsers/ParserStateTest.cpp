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