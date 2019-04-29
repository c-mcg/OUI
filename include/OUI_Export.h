#ifndef OUI_API

    #ifdef _WIN32
        #define OUI_API __declspec(dllexport)
    #elif defined __linux
        #define OUI_API __attribute__((visibility("default")))
    #else
        #define OUI_API
    #endif

#include <string>

#include "OUI_Constants.h"

#endif

