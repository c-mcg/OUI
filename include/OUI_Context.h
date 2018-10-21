#ifndef OUI_CONTEXT_H
#define OUI_CONTEXT_H

#include "OUI_Export.h"
#include "oui/OUI_Window.h"
#include <vector>

namespace oui {

	class OUI_API Context {

		private: std::vector<Window*> windows;

		private: long long lastSDLEvent;

		public: Context();

		public: int process();

		private: Window* getWindow(int sdlID);
		public: Window* getWindow(std::string name);
		public: void addWindow(Window* window);
		public: bool removeWindow(Window* window);
		public: bool deleteWindow(std::string name);

	};

}

#endif