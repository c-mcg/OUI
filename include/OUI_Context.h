#ifndef OUI_CONTEXT_H
#define OUI_CONTEXT_H

#include "OUI_Export.h"
#include "oui/OUI_Window.h"
#include <vector>

namespace oui {

	class OUI_API Context {

		protected: std::vector<Window*> windows;

		protected: long long lastEvent;

		public: Context();

		public: int process();

        protected: virtual bool pollEvents();

        public: virtual Window* createWindow(int width, int height);

		public: Window* getWindow(std::string name);
		public: Window* addWindow(Window* window);
		public: bool removeWindow(Window* window);
		public: bool deleteWindow(std::string name);

	};

}

#endif