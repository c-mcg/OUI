#include <iostream>

// #include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "OUI.h"
#include "util/OUI_StringUtil.h"

// All test files are found by CMake!
int main(int argc, char** argv) 
{ 
    testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS(); 
}