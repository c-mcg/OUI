#ifndef OUI_MOUSE_EVENT_H
#define OUI_MOUSE_EVENT_H

#include "OUI_Export.h"
#include "event/OUI_ComponentEvent.h"

namespace oui {

	class Window;

    class OUI_API MouseEvent : public ComponentEvent {
        public:

            static MouseEvent* create(std::string type, bool bubbles, Window* window, int button, std::vector<int> buttons, int movementX, int movementY);
            
            static MouseEvent* createBubbledEvent(MouseEvent* event);

            /**
             * @brief True if the `alt` key was down when the event was fired
             */
            const bool altKey;

            /**
             * @brief The number for the mouse button that triggered the event (if applicable)
             */
            const int button;

            /**
             * @brief Any mouse buttons held down when the event was fired
             */
            const std::vector<int> buttons;
        
            /**
             * @brief The X coordinate of the mouse relative to the target component 
             */
            const int localX;

            /**
             * @brief The Y coordinate of the mouse relative to the target component 
             */
            const int localY;

            /**
             * @brief Alias for `localX`
             */
            const int clientX;

            /**
             * @brief Alias for `localY`
             */
            const int clientY;

            /**
             * @brief True if the `ctrl` key was held when the even was fired
             */
            const bool ctrlKey;

            /**
             * @brief True if the `meta` key was held when the even was fired
             */
            const bool metaKey;

            /**
             * @brief The X coordinate of the mouse relative to the last mouse move event
             */
            const int movementX;

            /**
             * @brief The Y coordinate of the mouse relative to the last mouse move event
             */
            const int movementY;

            // /**
            //  * @brief The X coordinate of the mouse pointer relative to the position of the padding edge of the component
            //  */
            // const int offsetX;

            // /**
            //  * @brief The Y coordinate of the mouse pointer relative to the position of the padding edge of the component
            //  */
            // const int offsetY;

            /**
             * @brief Alias for `windowX`
             */
            const int pageX;

            /**
             * @brief Alias for `windowY`
             */
            const int pageY;

            // /**
            //  * @brief The amount of pressure applied (Tablet & mobile)
            //  */
            // const int pressure;

            // /**
            //  * @brief The id of the Component region clicked
            //  */
            // const std::string region; 

            // /**
            //  * @brief The secondary target for the event (if applicable)
            //  */
            // const Component* relatedTarget;

            /**
             * @brief The X coordinate of the mouse relative to the computer screen
             */
            const int screenX;
            
            /**
             * @brief The Y coordinate of the mouse relative to the computer screen
             */
            const int screenY;

            /**
             * @brief True if the `shift` key was held when the event was fired
             */
            const bool shiftKey;

            /**
             * @brief Alias for `button`
             */
            const int which;

            /**
             * @brief The X coordinate of the mouse relative to the window
             */
            const int windowX;

            /**
             * @brief The Y coordinate of the mouse relative to the window
             */
            const int windowY;

            /**
             * @brief Alias for `localX`
             */
            const int x;

            /**
             * @brief Alias for `localY`
             */
            const int y;

            /**
             * @brief Construct a new Mouse Event object
             * 
             * @param type The type of event
             * @param bubbles True if the event should bubble to parent components
             * @param originalTarget The component that originally fired the event before bubbling
             * @param altKey True if `alt` key is pressed down
             * @param button The number for the mouse button that triggered the event
             * @param buttons Any mouse buttons held down
             * @param ctrlKey True if `ctrl` key is held down
             * @param metaKey True if `meta` key is held down
             * @param shiftKey True if `shift` is held down
             * @param windowX The mouse X position relative to the window
             * @param windowY The mouse Y position relative to the window
             */
            MouseEvent(std::string type, bool bubbles, Component* originalTarget, bool altKey, int button, std::vector<int> buttons, int localX, int localY, bool ctrlKey, bool metaKey, int movementX, int movementY, int screenX, int screenY, bool shiftKey, int windowX, int windowY);
    };

}
#endif