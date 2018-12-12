

#ifndef OUI_OSX_H
#define OUI_OSX_H

#include "os/OUI_OS.h"

namespace oui {

	class Window;

	class OSX : public OperatingSystem {

        public: OSX();

        public: virtual void onAddWindow(Window* window);
        private: void enableInertiaScrolling();
        
		public: virtual bool initialize();

		public: virtual bool getGlobalMousePos(Window* window, int &x, int &y);

		public: virtual bool getMaximizeSize(Window* window, int &x, int &y, int &width, int &height);

	};

}

#endif