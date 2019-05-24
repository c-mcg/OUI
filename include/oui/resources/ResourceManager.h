
#ifndef OUI_COLOR_H
#define OUI_COLOR_H

#include "OUI_Export.h"
#include <stdint.h>
#include <map>
#include <string>

namespace oui {

    template <class T>
    class OUI_API ResourceManager {
        private: std::unordered_map<std::string, T>;

        public: ResourceManager();

        public:int getARGB();
    };

}

#endif