#ifndef OUI_OS_H
#define OUI_OS_H

#include "OUI_Export.h"
#include "exception/OUI_Exception.h"

namespace oui {

    class Context;
    class Window;

    class OUI_API OperatingSystem {

        public: OperatingSystem();

        public: virtual void onAddWindow(Window* window);

        public: virtual bool initialize();

        public: bool getGlobalMousePos(Window* window, int &x, int &y);

        public: virtual bool getMaximizeSize(Window* window, int &x, int &y, int &width, int &height);

        public: virtual Context* createContext();

        public: virtual void showErrorMessage(Exception e);

    };


}

#endif