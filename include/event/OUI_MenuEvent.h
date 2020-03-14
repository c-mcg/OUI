#ifndef OUI_MENU_EVENT_H
#define OUI_MENU_EVENT_H

#include "OUI_Export.h"
#include "event/OUI_ComponentEvent.h"
#include "component/impl/menu/OUI_Menu.h"

namespace oui {

    class Window;
	class Component;

    class OUI_API MenuEvent : public ComponentEvent {
        public:

            static MenuEvent* create(std::string type, Component* originalTarget, Menu* menu, int optionIndex, std::u16string option);

            Menu* const menu;
            const int optionIndex;
            const std::u16string option;

            /**
             * @brief Construct a new Menu Event object
             * 
             * @param type The type of menu event
             * @param originalTarget The component the menu is targeting (or NULL)
             * @param menu The menu that triggered the event
             * @param optionIndex The index of the option that triggered the event
             * @param option The text of the option that triggered the event
             */
            MenuEvent(std::string type, Component* originalTarget, Menu* menu, int optionIndex, std::u16string option);
    };

}
#endif