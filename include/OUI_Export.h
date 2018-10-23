#pragma once

#ifndef OUI_API

	#ifdef _WIN32
		#define OUI_API __declspec(dllexport)
	#else
		#define OUI_API
	#endif

#include <string>


#include "OUI_Constants.h"

#endif

