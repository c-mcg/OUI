#include "OUI_Export.h"

#ifndef OUI_WINDOWSOS_H
#define OUI_WINDOWSOS_H

#include "os/OUI_OS.h"

namespace oui {

    class Window;

    class OUI_API WindowsOS : public OperatingSystem {

        public: WindowsOS();
        
        public: bool initialize() override;

        public: bool getMaximizeSize(Window* window, int &x, int &y, int &width, int &height) override;

    };

}

#endif
