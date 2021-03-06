#ifndef OUI_EVENT_H
#define OUI_EVENT_H

#include "OUI_Export.h"

namespace oui {

    class OUI_API Event {
        public:

            /**
             * @brief Checks if the types for the specified event are equal. This is faster than raw string comparison.
             * 
             * @param event1 Event to compare
             * @param event2 Event to compare
             * @return true Events have the same type
             * @return false Events do not have the same type
             */
            static bool compareTypes(const Event& event1, const Event& event2);

            /**
             * @brief The type of event
             */
            const std::string eventClass;

            /**
             * @brief The type of event
             */
            const std::string type;

            /**
             * @brief Creates a new event with the specified type
             * 
             * @param type The type of evemt
             */
            explicit Event(std::string eventClass, std::string type);

            /**
             * @brief Checks if the type of event is the same as is this one. This is faster than raw string comparison.
             * 
             * @param event The event to compare with
             * @return true The event type is the same as this one
             * @return false The event type is not the same as this one
             */
            bool compareType(const Event& event);

            /**
             * @brief Checks if the type of event is the same as is this one. This is slower than comparing events or hashes.
             * 
             * @param event The event to compare with
             * @return true The event type is the same as this one
             * @return false The event type is not the same as this one
             */
            bool compareType(std::string event);

            /**
             * @brief Prevent any built-in behaviours the event may cause
             */
            void preventDefault();

            /**
             * @brief Returns true if default behaviour should be prevented
             * 
             * @return true
             * @return false
             */
            bool isDefaultPrevented();

        private:

            /**
             * @brief True if default behaviour should be prevented
             */
            bool defaultPrevented;

    };

}
#endif