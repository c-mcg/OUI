#ifndef OUI_STYLE_H
#define OUI_STYLE_H
#include "OUI_Export.h"
#include "OUI_AttributeProfile.h"
#include "osal/OSAL.h"

namespace oui {

	class OUI_API Style {
		private: std::unordered_map<std::u16string, AttributeProfile*> profiles;

		public: ~Style();

		public: Style();

		public: void addProfile(std::u16string name, AttributeProfile* profile);

		public: bool hasProfile(std::u16string name);
        
        public: std::vector<std::u16string> getProfileNames();

		public: AttributeProfile* getProfile(std::u16string name);

		public: AttributeProfile* getOrCreateProfile(std::u16string profile);

		//Adds the specified styles profiles and attributes to this style
		//Overwrites if `overwrite` is true
		public: void combineStyle(Style* style, bool overwrite = false);

		public: bool equals(Style* other);
		public: Style* clone();
	};

}

#endif