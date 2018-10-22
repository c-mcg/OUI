#pragma once

#ifndef OUI_API

	#ifdef OUI_EXPORTS
		#define OUI_API __declspec(dllexport)
	#else
		#ifdef _WIN32
			#define OUI_API __declspec(dllexport)
		#endif
	#endif

#include <string>


#include "OUI_Constants.h"

#endif

