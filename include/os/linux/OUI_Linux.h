#include "OUI_Export.h"

#ifndef OUI_LINUXOS_H
#define OUI_LINUXOS_H

#include "os/OUI_OS.h"

namespace oui {

    class Window;

    class OUI_API LinuxOS : public OperatingSystem {

        public: LinuxOS();
        
        public: bool initialize() override;

        public: bool getMaximizeSize(Window* window, int &x, int &y, int &width, int &height) override;

    };

}

#endif
