#include "os/OUI_OS.h"

#include "oui/OUI_Window.h"
#include "sdl2/OUI_SDL_Context.h"

#include <SDL.h>

oui::OperatingSystem::OperatingSystem() {

}

void oui::OperatingSystem::onAddWindow(Window* window) {
}

bool oui::OperatingSystem::initialize() {
    return true;
}

bool oui::OperatingSystem::getGlobalMousePos(Window* window, int &x, int &y) {
    SDL_GetGlobalMouseState(&x, &y);
    return true;
}
bool oui::OperatingSystem::getMaximizeSize(Window* window, int &x, int &y, int &width, int &height) {
    x = 0;
    y = 0;
    return true;
}

oui::Context* oui::OperatingSystem::createContext() {
    return new SDLContext();
}