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

		private: std::string name;//Name of the attribute
		private: std::vector<std::u16string> value;//Value of the attribute (split by spaces)
		private: std::vector<char> types;//The type of the value
		private: std::u16string originalString;

		public: Attribute(const std::string& name, const std::u16string& value);

		public: std::string getName();
		public: int getNumValues();//Returns the size of the value vector
		public: char getType(int index = 0);
		public: std::u16string getAsString(int index = 0);//Gets the attributes value (of the specified index) as a string
		public: int getAsInt(int index = 0);//Gets the attributes value (of the specified index) as an int
		public: double getAsDouble(int index = 0);//Gets the attributes value (of the specified index) as a double
		public: bool getAsBool(int index = 0);//Gets the attributes value (of the specified index) as a bool

		public: std::u16string getOriginalString();

		public: bool isValid();

	};
}

#endif