#ifndef OUI_EVENT_DISPATCHER_H
#define OUI_EVENT_DISPATCHER_H

#include "OUI_Export.h"
#include "event/OUI_ComponentEvent.h"
#include "event/OUI_MouseEvent.h"
#include <functional>

namespace oui {
	class Component;

    /**
     * @brief A function that handles an event
     */
    typedef std::function<void(ComponentEvent*)> EventHandler;

    /**
     * @brief Dispatches events to event listeners on the component
     */
    class OUI_API EventDispatcher {
        public:
        
            /**
             * @brief Construct a new Event Dispatcher object
             * 
             * @param target The component to dispatch events for 
             */
            EventDispatcher();

            /**
             * @brief Dispatches an event to the component.
             * This function will bubble the event to the component's parent if necessary.
             * 
             * @param event The event to dispatch 
             */
            virtual void dispatchEvent(ComponentEvent* event);

            /**
             * @brief Set the target component to dispatch events to. This function can only be called once 
             * 
             * @param target The target component 
             */
            void setTarget(Component* target);

            /**
             * @brief Adds a handler to the specified event.
             * 
             * @param typeHash A preoptimized hashed string type for the event type
             * @param handler A function to be called when the event occurs.
             */
            void addEventListener(std::size_t typeHash, EventHandler handler);
            
            /**
             * @brief Adds a handler to the specified event.
             * 
             * @param type The type of event to listen for
             * @param handler A function to be called when the event occurs.
             */
            void addEventListener(std::string type, EventHandler handler);

        protected:

            /**
             * @brief The component to dispatch events for
             */
            Component* target;

            /**
             * @brief Any listeners attached to the component
             */
            std::unordered_map<std::size_t, std::vector<EventHandler>> listeners;

            /**
             * @brief Triggers all event listeners relevant to the specified event 
             * 
             * @param event The event to trigger listeners for
             */
            void triggerListeners(ComponentEvent* event);
    };

}
#endif