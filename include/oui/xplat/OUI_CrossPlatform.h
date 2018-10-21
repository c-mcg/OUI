#ifndef OUI_CROSSPLATFORM_H
#define OUI_CROSS_PLATFORM_H

namespace oui {

	class Window;

	class CrossPlatform {

		public: static bool getGlobalMousePos(oui::Window* window, int &x, int &y);

		public: static bool getMaximizeSize(oui::Window* window, int &x, int &y, int &width, int &height);

	};

}

#endif