#ifndef OUI_KEYBOARD_EVENT
#define OUI_KEYBOARD_EVENT

#include "OUI_Export.h"
#include "event/OUI_ComponentEvent.h"

namespace oui {

	class Window;

    class OUI_API KeyboardEvent : public ComponentEvent {
        public:

            static KeyboardEvent* create(std::string type, bool bubbles, Window* window, int key);
            
            /**
             * @brief True if the `alt` key was down when the event was fired
             */
            const bool altKey;

            /**
             * @brief A number representing the key that fired the event
             */
            const int key;

            /**
             * @brief A text representation of the key that fired the event
             */
            const char character;

            /**
             * @brief True if the `ctrl` key was held when the even was fired
             */
            const bool ctrlKey;

            /**
             * @brief True if the `meta` key was held when the even was fired
             */
            const bool metaKey;

            /**
             * @brief True if the `shift` key was held when the event was fired
             */
            const bool shiftKey;

            /**
             * @brief Alias for `key`
             */
            const int which;

            /**
             * @brief Construct a new Mouse Event object
             * 
             * @param originalTarget The component that originally fired the event before bubbling
             * @param type The type of event
             * @param bubbles True if the event should bubble to parent components
             * @param altKey True if `alt` key is pressed down
             * @param button The number for the mouse button that triggered the event
             * @param buttons Any mouse buttons held down
             * @param ctrlKey True if `ctrl` key is held down
             * @param metaKey True if `meta` key is held down
             * @param shiftKey True if `shift` is held down
             * @param windowX The mouse X position relative to the window
             * @param windowY The mouse Y position relative to the window
             */
            KeyboardEvent(std::string type, bool bubbles, Component* originalTarget, bool altKey, bool ctrlKey, bool metaKey, bool shiftKey, int key);
    
    };

}
#endif