#include "oui/xplat/OUI_CrossPlatform.h"
#include "oui/OUI_Window.h"
#include "util/OUI_StringUtil.h"

#ifdef __linux__ 

	//LINUX IMPORTS HERE

#elif __APPLE__
	#include "TargetConditionals.h"

	#ifdef TARGET_OS_MAC

		//MAC IMPORTS HERE

	#endif

#elif defined _WIN32 || defined _WIN64
	#include <Windows.h>
	//Windows imports here

#endif




bool oui::CrossPlatform::getGlobalMousePos(Window* window, int &x, int &y) {
	#ifdef __linux__//LINUX

	//LINUX IMPORTS HERE

	#elif __APPLE__

		#include "TargetConditionals.h"
		#ifdef TARGET_OS_MAC//MAC

			//MAC IMPORTS HERE

		#endif

	#elif defined _WIN32 || defined _WIN64//WINDOWS

		POINT globalMousePos;//TODO check for possible errors and return false
		GetCursorPos(&globalMousePos);
		x = (int) globalMousePos.x;
		y = (int) globalMousePos.y;
		return true;

	#endif

	return false;
}

bool oui::CrossPlatform::getMaximizeSize(Window* window, int &x, int &y, int &width, int &height) {
	#ifdef __linux__//LINUX

	//LINUX IMPORTS HERE

	#elif __APPLE__
	#include "TargetConditionals.h"
	#ifdef TARGET_OS_MAC//MAC

		//MAC IMPORTS HERE

	#endif

	#elif defined _WIN32 || defined _WIN64//WINDOWS

		int taskBarHeight = 0;
		RECT rect;
		HWND taskBar = FindWindow(L"Shell_traywnd", NULL);
		if (taskBar && GetWindowRect(taskBar, &rect)) {
			taskBarHeight = rect.bottom - rect.top;
		}


		RECT* prc = new RECT();
		RECT rc;
		prc->left = window->getX();
		prc->top = window->getY();
		prc->right = window->getX() + window->getWidth();
		prc->bottom = window->getY() + window->getHeight();
		HMONITOR hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi;
		mi.cbSize = sizeof(mi);
		GetMonitorInfo(hMonitor, &mi);;
		rc = mi.rcMonitor;
		*prc = rc;

		x = prc->left;
		y = prc->top;
		width = prc->right - prc->left;
		height = prc->bottom - prc->top - taskBarHeight;

		delete prc;

		return true;

	#endif

	return false;
}