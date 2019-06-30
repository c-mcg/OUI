#ifndef OUI_EVENT2_H
#define OUI_EVENT2_H

#include "OUI_Export.h"

namespace oui {

    class OUI_API Event2 {
        public:

            /**
             * @brief Creates a hash for a type string
             * 
             * @param type The type string to hash
             * @return std::size_t The resulting `typeHash`
             */
            static std::size_t createTypeHash(std::string type);

            /**
             * @brief Checks if the types for the specified event are equal. This is faster than raw string comparison.
             * 
             * @param event1 Event to compare
             * @param event2 Event to compare
             * @return true Events have the same type
             * @return false Events do not have the same type
             */
            static bool compareTypes(const Event2& event1, const Event2& event2);

            /**
             * @brief A hasher held to optimize hashing
             */
            static std::hash<std::string> hasher;

            /**
             * @brief The type of event
             */
            const std::string type;

            /**
             * @brief Creates a new event with the specified type
             * 
             * @param type The type of evemt
             * @param typeHash an optional hashed version of `type`, which can be used for optimization
             */
            explicit Event2(std::string type, std::size_t typeHash=0);

            /**
             * @brief Checks if the `typeHash` same as is this one. This is faster than raw string comparison.
             * 
             * @param event The event to compare with
             * @return true The event type is the same as this one
             * @return false The event type is not the same as this one
             */
            bool compareType(std::size_t typeHash);

            /**
             * @brief Checks if the type of event is the same as is this one. This is faster than raw string comparison.
             * 
             * @param event The event to compare with
             * @return true The event type is the same as this one
             * @return false The event type is not the same as this one
             */
            bool compareType(const Event2& event);

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
             * @brief A hash of the type for quicker comparisons
             */
            std::size_t typeHash; 

            /**
             * @brief True if default behaviour should be prevented
             */
            bool defaultPrevented;

    };

}
#endif