#ifndef OSAL_ATTRIBUTE_H
#define OSAL_ATTRIBUTE_H
#include <vector>
#include "OUI_Export.h"

#include "gfx/OUI_Color.h"

/*
* This class represents an attribute on an element
*/
namespace OSAL {

    static const char TYPE_STRING = 0;
    static const char TYPE_INT = 1;
    static const char TYPE_DOUBLE = 2;
    static const char TYPE_BOOL = 3;
    static const char TYPE_COLOR = 4;
    class Attribute {

        public:

            static const char STRING = TYPE_STRING;
            static const char INT = TYPE_INT;
            static const char DOUBLE = TYPE_DOUBLE;
            static const char BOOL = TYPE_BOOL;
            static const char COLOR = TYPE_COLOR;


            /**
             * Creates a new attribute with the specified name and value
             */
            Attribute(const std::string& name, const std::u16string& value);

            /**
             * Returns the name of the attribute
             */
            std::string getName();

            /**
             * Returns the number of values the attribute has
             */
            int getNumValues();//Returns the size of the value vector

            /**
             * Gets the type at the specified index
             * Zero is default index for 1 value attributes
             */
            char getType(int index = 0);

            /**
             * Gets the value at the specified index as a string
             * Zero is default index for one value attributes
             */
            std::u16string getAsString(int index = 0);//Gets the attributes value (of the specified index) as a string

            /**
             * Gets the value at the specified index as an integer
             * Zero is default index for one value attributes
             */
            int getAsInt(int index = 0);//Gets the attributes value (of the specified index) as an int

            /**
             * Gets the value at the specified index as a double
             * Zero is default index for one value attributes
             */
            double getAsDouble(int index = 0);

            /**
             * Gets the value at the specified index as a boolean
             * Zero is default index for one value attributes
             */
            bool getAsBool(int index = 0);

            /**
             * Gets the value at the specified index as a Color
             * Zero is default index for one value attributes
             */
            oui::Color getAsColor(int index = 0);

            /**
             * Gets the original value string
             */
            std::u16string getOriginalString();

            /**
             * If false, there was an error parsing the attribute values
             */
            bool isValid();

         
        private:

            /**
             * The name of the attribute
             */
            std::string name;

            /**
             * The type of each value
             */
            std::vector<char> types;

            /**
             * The values for the attribute
             */
            std::vector<std::u16string> value;

            /**
             * The original string that the value is derived from
             */
            std::u16string originalString;

            void addValue(char type, std::u16string value);

    };
}

#endif