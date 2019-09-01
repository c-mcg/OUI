#include <SDL.h>

#include "os/OUI_OS.h"
#include "sdl2/OUI_SDL_Context.h"

#include <iostream>
oui::SDLContext::SDLContext() :
    lastSDLEvent{0}, capturedEvents{std::unordered_map<SDLWindow*, std::vector<SDL_Event>>()},
    Context()
{
}

oui::Window* oui::SDLContext::createWindow(int width, int height) {
    Window* window = new SDLWindow(width, height);
    return window;
}

bool oui::SDLContext::captureEvents() {
    SDL_Event event;
    SDLWindow* window = NULL;
    bool hasEvent = SDL_PollEvent(&event);
    const bool hadEvent = hasEvent;
    while (hasEvent) {
        if (event.type == SDL_WINDOWEVENT) {
            window = getWindow(event.window.windowID);
        }

        if (event.type == SDL_MOUSEBUTTONUP) {
            window = getWindow(event.button.windowID);
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            window = getWindow(event.button.windowID);
        }

        if (event.type == SDL_MOUSEMOTION) {
            window = getWindow(event.motion.windowID);
        }

        if (event.type == SDL_MOUSEWHEEL) {
            window = getWindow(event.wheel.windowID);
        }

        if (event.type == SDL_KEYDOWN) {
            window = getWindow(event.key.windowID);
        }
        if (event.type == SDL_KEYUP) {
            window = getWindow(event.key.windowID);
        }

        if (window != NULL) {
            auto it = capturedEvents.find(window);
            if (it != capturedEvents.end()) {
                std::vector<SDL_Event> events = it->second;
                events.push_back(event);
                capturedEvents.insert_or_assign(window, events);
            } else {
                std::vector<SDL_Event> events;
                events.push_back(event);
                capturedEvents.insert_or_assign(window, events);
            }
        }

        hasEvent = SDL_PollEvent(&event);
    }

    return hadEvent;
}


void oui::SDLContext::handleEventsForWindow(Window* baseWindow) {
    SDLWindow* window = static_cast<SDLWindow*>(baseWindow);
    auto mapIt = capturedEvents.find(window);

    if (mapIt == capturedEvents.end()) {
        return;
    }

    std::vector<SDL_Event> windowEvents = mapIt->second;

    for (auto vecIt = windowEvents.begin(); vecIt != windowEvents.end(); vecIt++) {
        SDL_Event event = *vecIt;
        window->handleSDLEvent(&event);
    }

    windowEvents.clear();
    capturedEvents.insert_or_assign(window, windowEvents);
}

oui::SDLWindow* oui::SDLContext::getWindow(int sdlID) {
    for (unsigned int i = 0; i < windows.size(); i++) {
        SDLWindow* window = static_cast<SDLWindow*>(windows.at(i));
        if (window->baseWindowId == sdlID) {
            return window;
        }
    }
    return NULL;
}