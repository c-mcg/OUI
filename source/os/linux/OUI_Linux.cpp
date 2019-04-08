
#if defined __linux

#include "os/linux/OUI_Linux.h"
#include <X11/Xlib.h>
#include "oui/OUI_Window.h"

oui::LinuxOS::LinuxOS() : OperatingSystem() {

}

bool oui::LinuxOS::initialize() {
    return true;
}

bool oui::LinuxOS::getGlobalMousePos(oui::Window* window, int &x, int &y) {
    auto rootw = XDefaultRootWindow(0);
    XEvent evt;
    int err = XGrabPointer(0,
                rootw,
                false,
                ButtonPressMask,
                GrabModeAsync,
                GrabModeAsync,
                None,
                0,
                CurrentTime);
    switch(err) {
        // ...
    }
    XNextEvent(0, &evt);
        // ...
    x = evt.xbutton.x_root;
    y = evt.xbutton.y_root;
    printf("Absolute coordinates: %d,%d\n", evt.xbutton.x_root, evt.xbutton.y_root);
    return true;
}
bool oui::LinuxOS::getMaximizeSize(oui::Window* window, int &x, int &y, int &width, int &height) {
    return true;
}

#endif