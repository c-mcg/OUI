#include "window/OUI_MouseManager.h"
#include "window/OUI_Window.h"
#include "event/OUI_ScrollEvent.h"
#include "component/impl/menu/OUI_Menu.h"
#include "OUI.h"
#include "component/attribute/OUI_AttributeNames.h"

using namespace oui::AttributeNames;

oui::MouseManager::MouseManager():
    globalMouseX{0}, globalMouseY{0}, lastGlobalMouseX{0}, lastGlobalMouseY{0},
    mouseX{0}, mouseY{0},
    window{NULL}
{

}

void oui::MouseManager::setWindow(Window* window) {
    if (this->window != NULL) {
        // TODO throw error
    }
    this->window = window;
}

int oui::MouseManager::getGlobalMouseX() {
    return globalMouseX;
}
int oui::MouseManager::getGlobalMouseY() {
    return globalMouseY;
}
int oui::MouseManager::getMouseX() {
    return mouseX;
}
int oui::MouseManager::getMouseY() {
    return mouseY;
}

void oui::MouseManager::onSystemMouseMove(ComponentEvent* compEvent) {
    MouseEvent* rawMouseEvent = static_cast<MouseEvent*>(compEvent);
    mouseX = rawMouseEvent->windowX;
    mouseY = rawMouseEvent->windowY;
    MouseEvent* event = MouseEvent::create("mousemove", true, window, rawMouseEvent->button, rawMouseEvent->buttons, rawMouseEvent->movementX, rawMouseEvent->movementY);

    bool resizing = window->isResizing();
    int windowX = window->getRelativeX();
    int windowY = window->getRelativeY();
    int windowWidth = window->getWidth();
    int windowHeight = window->getHeight();
    if (!resizing) {
        // TODO math util
        int dist = (int) abs(sqrt(pow(globalMouseX - windowX - windowWidth, 2) + pow(globalMouseY - windowY - windowHeight, 2)));
        if (dist < 15) {
            window->setCursor(constants::CURSOR_RESIZE);
        } else {
            window->setCursor(event->getTarget()->getCursor());
        }
    }


	window->setHovered(false);
	event->getTarget()->setHovered(true);

    window->getEventDispatcher()->dispatchEvent(event);
    delete event;
}

void oui::MouseManager::onSystemMouseUp(ComponentEvent* compEvent) {
    MouseEvent* rawMouseEvent = static_cast<MouseEvent*>(compEvent);
    mouseX = rawMouseEvent->windowX;
    mouseY = rawMouseEvent->windowY;

    MouseEvent* event = MouseEvent::create("mouseup", true, window, rawMouseEvent->button, rawMouseEvent->buttons, rawMouseEvent->movementX, rawMouseEvent->movementY);
    Component* c = event->getTarget();
    bool sendClickEvent = c->isMouseDown();
    window->stopResizing();
    window->stopMoving();
    window->setMouseDown(false);
    dispatchEvent(event);
    delete event;
    
    // Fire click event
    if (sendClickEvent) {
        MouseEvent* clickEvent = MouseEvent::create("click", true, window, rawMouseEvent->button, rawMouseEvent->buttons, rawMouseEvent->movementX, rawMouseEvent->movementY);
        dispatchEvent(clickEvent);
        delete clickEvent;
    }
}


void oui::MouseManager::onSystemMouseDown(ComponentEvent* compEvent) {
    MouseEvent* rawMouseEvent = static_cast<MouseEvent*>(compEvent);
    mouseX = rawMouseEvent->windowX;
    mouseY = rawMouseEvent->windowY;
    MouseEvent* event = MouseEvent::create("mousedown", true, window, rawMouseEvent->button, rawMouseEvent->buttons, rawMouseEvent->movementX, rawMouseEvent->movementY);
    Component* comp = event->getTarget();

    int windowX = window->getRelativeX();
    int windowY = window->getRelativeY();
    int windowWidth = window->getWidth();
    int windowHeight = window->getHeight();

    Menu* menu = static_cast<Menu*>(window->getChild("rightClickMenu"));
    if (menu != NULL && comp != menu) {
        menu->setAttribute(VISIBLE, false);
    }

    switch(event->button) {
        case 0: // Left Click
            int distToResize = (int) abs(sqrt(pow(globalMouseX - windowX -windowWidth, 2) + pow(globalMouseY - windowY - windowHeight, 2)));
            if (distToResize < 15) {
                window->startResizing();
                return;
            }
            break;
    }

    window->setMouseDown(false);
    comp->setMouseDown(true);
    window->setSelectedComponent(comp);
    
    dispatchEvent(event);
    delete event;
}

void oui::MouseManager::onSystemScrollWheel(ComponentEvent* compEvent) {
    ScrollEvent* rawScrollEvent = static_cast<ScrollEvent*>(compEvent);
    ScrollEvent* event = ScrollEvent::create(true, window, rawScrollEvent->buttons, rawScrollEvent->movementX, rawScrollEvent->movementY, rawScrollEvent->scrollDistance);
    dispatchEvent(event);
    delete event;
}

void oui::MouseManager::process() {
    int windowX = window->getRelativeX();
    int windowY = window->getRelativeY();
    int windowWidth = window->getWidth();
    int windowHeight = window->getHeight();
    OS()->getGlobalMousePos(window, globalMouseX, globalMouseY);

    bool mouseOutsideOfWindow = globalMouseX < windowX || globalMouseX > windowX + windowWidth ||
        globalMouseY < windowY || globalMouseY > windowY +windowHeight;
    if (mouseOutsideOfWindow &&  (globalMouseX != lastGlobalMouseX || globalMouseY != lastGlobalMouseY)) {
        MouseEvent* mouseEvent = new MouseEvent(
            "mousemove", true, NULL, false, 0,
            window->getMouseButtonsDown(),
            0, 0, false, false,
            globalMouseX - lastGlobalMouseX,
            globalMouseY - lastGlobalMouseY,
            globalMouseX, globalMouseY,
            false,
            windowX - globalMouseX,
            windowY - globalMouseY
        );
        onSystemMouseMove(mouseEvent);
    }

    lastGlobalMouseX = globalMouseX;
    lastGlobalMouseY = globalMouseY;
}

void oui::MouseManager::dispatchEvent(ComponentEvent* event) {
    window->getEventDispatcher()->dispatchEvent(event);
}