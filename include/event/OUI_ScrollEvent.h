#ifndef OUI_SCROLL_EVENT_H
#define OUI_SCROLL_EVENT_H

#include "OUI_Export.h"
#include "event/OUI_MouseEvent.h"

namespace oui {

    class Window;

    class OUI_API ScrollEvent : public MouseEvent {
        public:

            static ScrollEvent* create(bool bubbles, Window* window, std::vector<int> buttons, int movementX, int movementY, int scrollDistance);

            /**
             * @brief The number of scroll ticks. Polarity indicates direction
             */
            const int scrollDistance;

            /**
             * @brief Construct a new Mouse Event object
             * 
             * @param type The type of event
             * @param bubbles True if the event should bubble to parent components
             * @param originalTarget The component that originally fired the event before bubbling
             * @param altKey True if `alt` key is pressed down
             * @param buttons Any mouse buttons held down
             * @param ctrlKey True if `ctrl` key is held down
             * @param metaKey True if `meta` key is held down
             * @param shiftKey True if `shift` is held down
             * @param windowX The mouse X position relative to the window
             * @param windowY The mouse Y position relative to the window
             * @param scrollDistance The distance that has been scrolled
             */
            ScrollEvent(bool bubbles, Component* originalTarget, bool altKey, std::vector<int> buttons, int localX, int localY, bool ctrlKey, bool metaKey, int movementX, int movementY, int screenX, int screenY, bool shiftKey, int windowX, int windowY, int scrollDistance);
    };

}
#endif