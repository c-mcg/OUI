#include "event/OUI_KeyboardEvent.h"

#include "OUI_window.h"
#include "OUI_KeyCodes.h"

oui::KeyboardEvent* oui::KeyboardEvent::create(std::string type, bool bubbles, Window* window, int key) {
    Component* originalTarget = window->getSelectedComponent();

    return new KeyboardEvent(type, bubbles, originalTarget,
        window->isAltDown(),  window->isCtrlDown(),
        window->isMetaDown(), window->isShiftDown(), key);
}

oui::KeyboardEvent::KeyboardEvent(std::string type, bool bubbles, Component* originalTarget, bool altKey, bool ctrlKey, bool metaKey, bool shiftKey, int key):
    altKey{altKey}, ctrlKey{ctrlKey}, metaKey{metaKey},
    shiftKey{shiftKey}, which{key}, key{key}, character{getChar(key, shiftKey)},
    ComponentEvent(originalTarget, "keyboard", type, bubbles)
{

}