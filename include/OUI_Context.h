#ifndef OUI_CONTEXT_H
#define OUI_CONTEXT_H

#include "OUI_Export.h"
#include "OUI_Window.h"
#include <vector>

namespace oui {

    class Context {

        protected: std::vector<Window*> windows;

        protected: long long lastEvent;

        public: OUI_API Context();

        public: OUI_API int process();

        protected: virtual bool pollEvents();

        public: OUI_API virtual Window* createWindow(int width, int height);

        public: OUI_API Window* getWindow(const std::string& name);
        public: OUI_API Window* addWindow(Window* window);
        public: OUI_API bool removeWindow(Window* window);
        public: OUI_API bool deleteWindow(const std::string& name);

    };

}

#endif