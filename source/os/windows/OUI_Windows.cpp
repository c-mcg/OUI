
#if defined _WIN32 || defined _WIN64

#include "os/windows/OUI_Windows.h"

#include <Windows.h>
#include <iostream>
#include <sstream>

#include "window/OUI_window.h"

oui::WindowsOS::WindowsOS() : OperatingSystem() {

}

bool oui::WindowsOS::initialize() {
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

void oui::WindowsOS::showErrorMessage(Exception e) {
    std::wstring message;

    std::wstringstream wss;
    wss << L"OUI Exception \n"
        "File/Class: " << e.getFileName().c_str() << "\n" <<
        "Function: " << e.getFunctionName().c_str() << "\n" <<
        "Reason: " << e.getReason().c_str() << "\n" <<
        "Next Steps:: " << e.getHowToFix().c_str() << "\n";

    message.append( wss.str() );
    MessageBox(
        NULL,
        ws2s(message).c_str(),
        ws2s(L"Error Occured!").c_str(),
        MB_ICONERROR
    );
}


std::string oui::WindowsOS::ws2s(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0); 
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0); 
    return strTo;
}

#endif