#include "TestUtil.h"

#include "oui/comp/OUI_Button.h"

TEST(ParserState, itHasCorrectDefaultStyle)
{
    oui::Button button("test-button", "");
    button.setProfile("default");
    oui::AttributeProfile* defaultProfile = button.getCurrentProfile();

    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("text"), 
        u""
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("image"), 
        u""
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("font-face"), 
        u"notoserif"
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("font-size"), 
        14
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("cursor"), 
        u"pointer"
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("bg-color1-r"), 
        220
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("bg-color1-g"), 
        220
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("bg-color1-b"), 
        220
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("bg-color1-a"), 
        255
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("bg-color2-r"), 
        220
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("bg-color2-g"), 
        220
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("bg-color2-b"), 
        220
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("bg-color2-a"), 
        255
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("border-style"), 
        u"solid"
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("border-color-r"), 
        0
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("border-color-g"), 
        0
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("border-color-b"), 
        0
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("border-color-a"), 
        64
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("text-color-r"), 
        24
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("text-color-g"), 
        24
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("text-color-b"), 
        24
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("text-color-a"), 
        255
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("width-percent"), 
        0
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("height-percent"), 
        0
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("width-offset"), 
        125
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("height-offset"), 
        26
    );
    EXPECT_ATTRIBUTE_EQUAL(
        profile->getAttribute("center-text"), 
        true
    );
}