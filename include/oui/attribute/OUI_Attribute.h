#ifndef OUI_ATTRIBUTE_H
#define OUI_ATTRIBUTE_H

#include "OUI_Export.h"

namespace oui {

	class OUI_API Attribute {//TODO make seperate file
		public: char type;

		//TODO these should be stored in a subclass of Attribute
		//There would be no variables here, but the rest would stay the same
		public: int intVal = 0;
		public: std::u16string stringVal = u"";
		public: bool boolVal = false;
		public: double doubleVal = 0;

		public: ~Attribute();
		public: Attribute();
		public: Attribute(const Attribute& a);
		public: Attribute(int value);
		public: Attribute(std::string value);
		public: Attribute(const char* value);
		public: Attribute(std::u16string value);
		public: Attribute(const char16_t* value);
		public: Attribute(bool value);
		public: Attribute(double value);

		public: std::u16string toString();
		public: bool equals(Attribute other);
		public: Attribute* clone();
	};

}

#endif