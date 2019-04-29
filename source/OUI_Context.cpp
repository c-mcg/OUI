#include "OUI_Context.h"
#include "OUI.h"
#include "util/OUI_StringUtil.h"
#include "os/OUI_OS.h"

#include <iostream>

oui::Context::Context() :
    windows{std::vector<Window*>()}, lastEvent{0} {
}

int oui::Context::process() {

    if (windows.size() == 0) {
        std::cout << "No windows" << std::endl;
        return -1;
    }

    std::vector<Window*>::iterator it;
    for (it = windows.begin(); it != windows.end();) {//TODO make a safe(er) loop like the queuedEvents
        
        if ((*it)->process() == -1) {
        std::cout << "deleted the window" << std::endl;

            delete *it;
            it = windows.erase(it);
        } else {
            ++it;
        }
        
    }

    if (pollEvents()) {
        lastEvent = currentTimeMillis();
    }

    sleep(currentTimeMillis() - lastEvent > 5000 ? 32 : 0);
    return 0;
}

oui::Window* oui::Context::getWindow(const std::string& name) {
    for (unsigned int i = 0; i < windows.size(); i++) {
        if (windows.at(i)->getName() == name) {
            return windows.at(i);
        }
    }
    return NULL;
}

oui::Window* oui::Context::addWindow(Window* window) {
    windows.push_back(window);
    window->setContext(this);
    OS()->onAddWindow(window);
    return window;
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
bool oui::Context::deleteWindow(const std::string& name) {
    for (unsigned int i = 0; i < windows.size(); i++) {
        if (windows.at(i)->getName() == name) {
            delete windows.at(i);
            windows.erase(windows.begin() + i);
            return true;
        }
    }
    return false;
}

oui::Window* oui::Context::createWindow(int width, int height) {
    return NULL;
}

bool oui::Context::pollEvents() {
    return false;
}