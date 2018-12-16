#include <iostream>

#include "OUI.h"
#include "gtest/gtest.h"
#include "util/OUI_StringUtil.h"

static void EXPECT_SSTREQ(const std::string& expected, const std::string& actual) {
    EXPECT_STREQ(expected.c_str(), actual.c_str());
}

static void EXPECT_SSTRNE(const std::string& expected, const std::string& actual) {
    EXPECT_STRNE(expected.c_str(), actual.c_str());
}

static void EXPECT_SSTREQ16(const std::u16string& expected, const std::u16string& actual) {
    EXPECT_STREQ(convertUTF16toUTF8(expected).c_str(), convertUTF16toUTF8(actual).c_str());
}

// All test files are found by CMake!
int main(int argc, char** argv) 
{ 
    testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS(); 
    std::getchar(); // keep console window open until Return keystroke
}