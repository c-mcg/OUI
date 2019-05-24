
#include "gtest/gtest.h"

#include "OUI_Export.h"
#include "util/OUI_StringUtil.h"

#include "OUI_Attribute.h"

static void EXPECT_STR_EQUAL(const std::string& expected, const std::string& actual) {
    EXPECT_STREQ(expected.c_str(), actual.c_str());
}

static void EXPECT_ATTRIBUTE_EQUAL(oui::Attribute attr1, oui::Attribute attr2) {
    ASSERT_TRUE(attr1.equals(attr2));
}

static void EXPECT_STR_NOT_EQUAL(const std::string& expected, const std::string& actual) {
    EXPECT_STRNE(expected.c_str(), actual.c_str());
}

static void EXPECT_STR16_EQUAL(const std::u16string& expected, const std::u16string& actual) {
    EXPECT_STREQ(convertUTF16toUTF8(expected).c_str(), convertUTF16toUTF8(actual).c_str());
}

static void EXPECT_STR16_NOT_EQUAL(const std::u16string& expected, const std::u16string& actual) {
    EXPECT_STRNE(convertUTF16toUTF8(expected).c_str(), convertUTF16toUTF8(actual).c_str());
}
