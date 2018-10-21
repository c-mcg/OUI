#include "OUI_Context.h"
#include "OUI.h"
#include <SDL.h>
#include "util/OUI_StringUtil.h"

oui::Context::Context() {
	windows = std::vector<Window*>();
}

int oui::Context::process() {
	if (windows.size() == 0) {
		return -1;
	}
	long long now = currentTimeMillis();

	std::vector<Window*>::iterator it;
	for (it = windows.begin(); it != windows.end();) {//TODO make a safe(er) loop like the queuedEvents
		if ((*it)->process() == -1) {
			delete *it;
			it = windows.erase(it);
		} else {
			++it;
		}
	}

	SDL_Event event;
	Window* window = NULL;
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

	if (hadEvent) {
		lastSDLEvent = currentTimeMillis();
	}
	//int wait = (int) (currentTimeMillis() - start);
	//if (wait > 5) {
		//std::cout << "process time took: " << wait << std::endl;
	//}

	oui::sleep(currentTimeMillis() - lastSDLEvent > 5000 ? 32 : 0);
	return 0;
}

oui::Window* oui::Context::getWindow(int sdlID) {
	for (unsigned int i = 0; i < windows.size(); i++) {
		if (windows.at(i)->baseWindowID == sdlID) {
			return windows.at(i);
		}
	}
	return NULL;
}
oui::Window* oui::Context::getWindow(std::string name) {
	for (unsigned int i = 0; i < windows.size(); i++) {
		if (windows.at(i)->getName() == name) {
			return windows.at(i);
		}
	}
	return NULL;
}
void oui::Context::addWindow(Window* window) {
	windows.push_back(window);
	window->setContext(this);
}
bool oui::Context::removeWindow(Window* window) {
	for (unsigned int i = 0; i < windows.size(); i++) {
		if (windows.at(i) == window) {
			windows.at(i)->setContext(NULL);
			windows.erase(windows.begin() + i);
			return true;
		}
	}
	return false;
}
bool oui::Context::deleteWindow(std::string name) {
	for (unsigned int i = 0; i < windows.size(); i++) {
		if (windows.at(i)->getName() == name) {
			delete windows.at(i);
			windows.erase(windows.begin() + i);
			return true;
		}
	}
	return false;
}