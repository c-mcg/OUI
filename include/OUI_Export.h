#pragma once

#ifndef OUI_API

	#ifdef OUI_EXPORTS
		#define OUI_API __declspec(dllexport)
	#else
		#define OUI_API __declspec(dllexport)
	#endif

#include <string>


#include "OUI_Constants.h"

#endif

