#include <iostream>

#include "OUI.h"
#include "gtest/gtest.h"
#include "util/OUI_StringUtil.h"

// All test files are found by CMake!
int main(int argc, char** argv) 
{ 
    testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS(); 
}