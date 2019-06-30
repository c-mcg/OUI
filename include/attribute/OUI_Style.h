#ifndef OUI_STYLE_H
#define OUI_STYLE_H
#include "OUI_Export.h"
#include "OUI_AttributeProfile.h"
#include "osal/OSAL.h"

namespace oui {

    class Style {
        private: std::unordered_map<std::u16string, AttributeProfile*> profiles;

        public: OUI_API ~Style();

        public: OUI_API Style();

        public: OUI_API void addProfile(const std::u16string& name, AttributeProfile* profile);

        public: OUI_API bool hasProfile(const std::u16string& name);
        
        public: OUI_API std::vector<std::u16string> getProfileNames();

        public: OUI_API AttributeProfile* getProfile(const std::u16string& name);

        public: OUI_API AttributeProfile* getOrCreateProfile(const std::u16string& profile);

        //Adds the specified styles profiles and attributes to this style
        //Overwrites if `overwrite` is true
        public: OUI_API void combineStyle(Style* style, bool overwrite = false);

        public: OUI_API bool equals(Style* other);
        public: OUI_API Style* clone();
    };

}

#endif