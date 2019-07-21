#ifndef OUI_WINDOW_EVENT_H
#define OUI_WINDOW_EVENT_H

#include "OUI_Export.h"
#include "event/OUI_ComponentEvent.h"

namespace oui {

    class Window;

    class OUI_API WindowEvent : public ComponentEvent {
        public:

            static WindowEvent* create(std::string type, Window* window, int newX, int newY, int newWidth, int newHeight);

            /**
             * @brief The number of scroll ticks. Polarity indicates direction
             */
            Window* const window;

            const int newX;
            const int newY;
            const int newWidth;
            const int newHeight;

            /**
             * @brief Construct a new Mouse Event object
             * 
             * @param type The type of event
             * @param window The window that triggered the event
             */
            WindowEvent(std::string type, Window* window, int newX, int newY, int newWidth, int newHeight);
    };

}
#endif