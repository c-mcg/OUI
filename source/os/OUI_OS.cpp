#include "os/OUI_OS.h"

#include "window/OUI_window.h"
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
    SDL_GetGlobalMouseState(&x, &y); // TODO why is SDL here?
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

void oui::OperatingSystem::showErrorMessage(Exception e) {
    // TODO log error
}