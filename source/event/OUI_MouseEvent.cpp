#include "event/OUI_MouseEvent.h"

#include "OUI_window.h"
#include "event/OUI_ScrollEvent.h"

oui::MouseEvent* oui::MouseEvent::create(std::string type, bool bubbles, Window* window, int button, std::vector<int> buttons, int movementX, int movementY) {
    int mouseX = window->getMouseX();
    int mouseY = window->getMouseY();
    Component* originalTarget = window->getComponentAt(mouseX, mouseY);
    
    int localX = mouseX - originalTarget->getScreenX();
    int localY = mouseY - originalTarget->getScreenY();

    return new MouseEvent(type, bubbles, originalTarget,
        window->isAltDown(), button, buttons, localX, localY, window->isCtrlDown(),
        window->isMetaDown(), movementX, movementY, window->getScreenX(), window->getScreenY(),
        window->isShiftDown(), mouseX, mouseY);
}


oui::MouseEvent* oui::MouseEvent::createBubbledEvent(MouseEvent* event) {
    auto comp = event->getTarget();
    auto parent = comp->getParent();
    int newLocalX = event->localX + comp->getX();
    int newLocalY = event->localY + comp->getY();

    if (event->type == "scroll") {
        int scrollDistance = ((ScrollEvent*) event)->scrollDistance;
        return new ScrollEvent(event->bubbles, parent, event->altKey, event->buttons, newLocalX, newLocalY, event->ctrlKey, event->metaKey, event->movementX, event->movementY, event->screenX, event->screenY, event->shiftKey, event->windowX, event->windowY, scrollDistance);
    }

    return new MouseEvent(event->type, event->bubbles, parent, event->altKey, event->button, event->buttons, newLocalX, newLocalY, event->ctrlKey, event->metaKey, event->movementX, event->movementY, event->screenX, event->screenY, event->shiftKey, event->windowX, event->windowY);
}

oui::MouseEvent::MouseEvent(std::string type, bool bubbles, Component* originalTarget, bool altKey, int button, std::vector<int> buttons, int localX, int localY, bool ctrlKey, bool metaKey, int movementX, int movementY, int screenX, int screenY, bool shiftKey, int windowX, int windowY):
    altKey{altKey}, button{button}, buttons{buttons}, localX{localX},
    localY{localY}, clientX{localX}, clientY{localY}, ctrlKey{ctrlKey}, metaKey{metaKey},
    movementX{movementX}, movementY{movementY}, pageX{windowX}, pageY{windowY},
    screenX{screenX}, screenY{screenY}, shiftKey{shiftKey}, which{which},
    windowX{windowX}, windowY{windowY}, x{x}, y{y},
    ComponentEvent(originalTarget, "mouse", type, bubbles)
{

}