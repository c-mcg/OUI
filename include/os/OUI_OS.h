#ifndef OUI_OS_H
#define OUI_OS_H

#include "OUI_Export.h"

namespace oui {

    class Context;
    class Window;

    class OUI_API OperatingSystem {

        public: OperatingSystem();

        public: virtual void onAddWindow(Window* window);

        public: virtual bool initialize();

        public: virtual bool getGlobalMousePos(Window* window, int &x, int &y);

        public: virtual bool getMaximizeSize(Window* window, int &x, int &y, int &width, int &height);

        public: virtual Context* createContext();
    };


}

#endif