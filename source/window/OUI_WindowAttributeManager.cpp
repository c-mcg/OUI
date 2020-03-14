#include "window/OUI_WindowAttributeManager.h"
#include "window/OUI_window.h"

oui::WindowAttributeManager::WindowAttributeManager():
    ContainerAttributeManager()
{

}

void oui::WindowAttributeManager::refreshProfile() {
    Window* window = static_cast<Window*>(component);
    bool wasVisible = isVisible();
    int width = window->getWidth();
    int height = window->getHeight();
    int x = window->getRelativeX();
    int y = window->getRelativeY();

    ContainerAttributeManager::refreshProfile();


    bool newVisible = isVisible();
    if (wasVisible != newVisible) {
        //setVisible(newVisible);
        if (newVisible) {
            window->showWindow();
        } else {
            window->hideWindow();
        }
    }

    
    if (width != window->getWidth() || height != window->getHeight()) {
        window->setSize(window->getWidth(), window->getHeight());
        window->flagGraphicsUpdateAll();
    }

    if (x != window->getRelativeX() || y != window->getRelativeY()) {
        window->setPosition(window->getRelativeX(), window->getRelativeY());
    }

}