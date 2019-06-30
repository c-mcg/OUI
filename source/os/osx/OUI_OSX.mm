#if __APPLE__
    #include "TargetConditionals.h"

    #ifdef TARGET_OS_MAC

#include <ApplicationServices/ApplicationServices.h>
#import <Foundation/Foundation.h>
#include <SDL.h>

#include "os/osx/OUI_OSX.h"

#include "OUI_Window.h"

oui::OSX::OSX() : OperatingSystem() {

}

bool oui::OSX::initialize() {
    
    return true;
}

void oui::OSX::onAddWindow(Window* window) {
    window->setTimeout(500, [this]() {//TODO thi could go in initialize
        this->enableInertiaScrolling();
    });
}

void oui::OSX::enableInertiaScrolling() {
    std::cout << "initialized OSX" << std::endl;
        NSDictionary *appDefaults = [
        [NSDictionary alloc] initWithObjectsAndKeys:
        [NSNumber numberWithBool:YES], @"AppleMomentumScrollSupported",
        //[NSNumber numberWithBool:NO], @"ApplePersistenceIgnoreState",
        //[NSNumber numberWithBool:YES], @"ApplePressAndHoldEnabled",
        nil
    ];
    [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
    [appDefaults release];
}

bool oui::OSX::getMaximizeSize(oui::Window* window, int &x, int &y, int &width, int &height) {
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    x = 0;
    y = 0;
    width = DM.w;
    height = DM.h;
    return true;
}

    #endif
#endif