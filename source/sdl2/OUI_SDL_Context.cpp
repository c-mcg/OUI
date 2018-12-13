#include <SDL.h>

#include "os/OUI_OS.h"
#include "sdl2/OUI_SDL_Context.h"

#include <iostream>
oui::SDLContext::SDLContext() : Context() {
	lastSDLEvent = 0;
	std::cout << "created sdl context" << std::endl;
}

oui::Window* oui::SDLContext::createWindow(int width, int height) {
    Window* window = new SDLWindow(width, height);
	std::cout << "Window created" << std::endl;

	//window->setContext(this);//TODO move to window constructor param
    return window;
}

bool oui::SDLContext::pollEvents() {
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
			window->handleSDLEvent(&event);
		}

		hasEvent = SDL_PollEvent(&event);
	}

	return hadEvent;
}

oui::SDLWindow* oui::SDLContext::getWindow(int sdlID) {
	for (unsigned int i = 0; i < windows.size(); i++) {
        SDLWindow* window = (SDLWindow*) windows.at(i);
		if (window->baseWindowId == sdlID) {
			return window;
		}
	}
	return NULL;
}