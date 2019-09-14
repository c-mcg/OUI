#include "event/OUI_MenuEvent.h"

#include "window/OUI_window.h"

oui::MenuEvent* oui::MenuEvent::create(std::string type, Component* originalTarget, Menu* menu, int optionIndex, std::u16string option) {
    return new MenuEvent(type, originalTarget, menu, optionIndex, option);
}

oui::MenuEvent::MenuEvent(std::string type, Component* originalTarget, Menu* menu, int optionIndex, std::u16string option):
    menu{menu}, optionIndex{optionIndex}, option{option},
    ComponentEvent(originalTarget, "menu", type, false)
{

}