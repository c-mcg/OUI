#include "event/OUI_ScrollEvent.h"

#include "OUI_window.h"

oui::ScrollEvent* oui::ScrollEvent::create(bool bubbles, Window* window, std::vector<int> buttons, int movementX, int movementY, int scrollDistance) {
    int mouseX = window->getMouseX();
    int mouseY = window->getMouseY();
    Component* originalTarget = window->getComponentAt(mouseX, mouseY);
    
    int localX = mouseX - originalTarget->getScreenX();
    int localY = mouseY - originalTarget->getScreenY();

    return new ScrollEvent(bubbles, originalTarget,
        window->isAltDown(), buttons, localX, localY, window->isCtrlDown(),
        window->isMetaDown(), movementX, movementY, window->getScreenX(), window->getScreenY(),
        window->isShiftDown(), mouseX, mouseY, scrollDistance);
}

oui::ScrollEvent::ScrollEvent(bool bubbles, Component* originalTarget, bool altKey, std::vector<int> buttons, int localX, int localY, bool ctrlKey, bool metaKey, int movementX, int movementY, int screenX, int screenY, bool shiftKey, int windowX, int windowY, int scrollDistance):
    scrollDistance{scrollDistance},
    MouseEvent("scroll", true, originalTarget, altKey, 1, buttons, localX, localY, ctrlKey, metaKey, movementX, movementY, screenX, screenY, shiftKey, windowX, windowY)
{

}