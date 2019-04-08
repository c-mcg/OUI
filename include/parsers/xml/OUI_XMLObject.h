#ifndef OUI_XMLOBJECT_H
#define OUI_XMLOBJECT_H

#include <unordered_map>
#include <vector>

#include "OUI_Export.h"

namespace oui {

    struct OUI_API XMLObject {
        std::string tag;
        std::unordered_map<std::string, std::u16string> attributes;
        std::vector<XMLObject> children;
        
        public: XMLObject(std::string tag, std::unordered_map<std::string, std::u16string> attributes);

        public: std::u16string getAttribute(std::string attribute);
        public: XMLObject getChild(int index);
    };

}

#endif
