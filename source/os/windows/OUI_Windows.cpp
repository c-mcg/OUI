
#if defined _WIN32 || defined _WIN64

#include "os/windows/OUI_Windows.h"

#include <Windows.h>

#include "OUI_Window.h"

oui::WindowsOS::WindowsOS() : OperatingSystem() {

}

bool oui::WindowsOS::initialize() {
    // if (LoadLibraryExA("sdlimg\\SDL2_image.dll", 0, 0) == NULL) {
    //     std::cout << "Couldn't load SDL2_image.dll" << std::endl;
    // }
    // if (LoadLibraryExA("sdlttf\\SDL2_ttf.dll", 0, 0) == NULL) {
    //     std::cout << "Couldn't load SDL2_ttf.dll" << std::endl;
    // }
    return true;
}

bool oui::WindowsOS::getMaximizeSize(oui::Window* window, int &x, int &y, int &width, int &height) {
    int taskBarHeight = 0;
    RECT rect;
    HWND taskBar = FindWindow("Shell_traywnd", NULL);
    if (taskBar && GetWindowRect(taskBar, &rect)) {
        taskBarHeight = rect.bottom - rect.top;
        //TODO task bar on side or top
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
}

#endif