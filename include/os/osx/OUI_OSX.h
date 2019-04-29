

#ifndef OUI_OSX_H
#define OUI_OSX_H

#include "os/OUI_OS.h"

namespace oui {

    class Window;

    class OSX : public OperatingSystem {

        public: OSX();

        public: void onAddWindow(Window* window);
        private: void enableInertiaScrolling();
        
        public: bool initialize();

        public: bool getGlobalMousePos(Window* window, int &x, int &y);

        public: bool getMaximizeSize(Window* window, int &x, int &y, int &width, int &height);

    };

}

#endif