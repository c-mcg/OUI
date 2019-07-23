#ifndef OUI_WINDOW_EVENT_DISPATCHER_H
#define OUI_WINDOW_EVENT_DISPATCHER_H

#include "OUI_Export.h"
#include "event/OUI_EventDispatcher.h"

namespace oui {

    /**
     * @brief Dispatches events to child components
     */
    class OUI_API WindowEventDispatcher : public EventDispatcher {
        public:
        
            /**
             * @brief Construct a new Window Event Dispatcher object
             * 
             * @param target The component to dispatch events for 
             */
            WindowEventDispatcher();

            /**
             * @brief Dispatches an event to the window.
             * This function will be dispatched to a child component if necessary.
             * 
             * @param event The event to dispatch 
             */
            virtual void dispatchEvent(ComponentEvent* event);

    };

}
#endif