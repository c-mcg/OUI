#ifndef OSAL_ATTRIBUTE_H
#define OSAL_ATTRIBUTE_H
#include <vector>
#include "OUI_Export.h"

/*
* This class represents an attribute on an element
*/
namespace OSAL {

    static const char TYPE_STRING = 0;
    static const char TYPE_INT = 1;
    static const char TYPE_DOUBLE = 2;
    static const char TYPE_BOOL = 3;
    class Attribute {

        public: static const char STRING = 0;
        public: static const char INT = 1;
        public: static const char DOUBLE = 2;
        public: static const char BOOL = 3;

        /**
         * The name of the attribute
         */
        private: std::string name;

        /**
         * The type of each value
         */
        private: std::vector<char> types;

        /**
         * The values for the attribute
         */
        private: std::vector<std::u16string> value;

        /**
         * The original string that the value is derived from
         */
        private: std::u16string originalString;

        /**
         * Creates a new attribute with the specified name and value
         */
        public: Attribute(const std::string& name, const std::u16string& value);

        /**
         * Returns the name of the attribute
         */
        public: std::string getName();

        /**
         * Returns the number of values the attribute has
         */
        public: int getNumValues();//Returns the size of the value vector

        /**
         * Gets the type at the specified index
         * Zero is default index for 1 value attributes
         */
        public: char getType(int index = 0);

        /**
         * Gets the value at the specified index as a string
         * Zero is default index for one value attributes
         */
        public: std::u16string getAsString(int index = 0);//Gets the attributes value (of the specified index) as a string

        /**
         * Gets the value at the specified index as an integer
         * Zero is default index for one value attributes
         */
        public: int getAsInt(int index = 0);//Gets the attributes value (of the specified index) as an int

        /**
         * Gets the value at the specified index as a double
         * Zero is default index for one value attributes
         */
        public: double getAsDouble(int index = 0);//Gets the attributes value (of the specified index) as a double

        /**
         * Gets the value at the specified index as a boolean
         * Zero is default index for one value attributes
         */
        public: bool getAsBool(int index = 0);//Gets the attributes value (of the specified index) as a bool

        /**
         * Gets the original value string
         */
        public: std::u16string getOriginalString();

        /**
         * If false, there was an error parsing the attribute values
         */
        public: bool isValid();

    };
}

#endif