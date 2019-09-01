#ifndef OUI_CONTEXT_H
#define OUI_CONTEXT_H

#include "OUI_Export.h"
#include "OUI_Window.h"
#include <vector>

namespace oui {

    class Context {

        public:
        
            OUI_API Context();

            OUI_API int process();

            OUI_API virtual Window* createWindow(int width, int height);

            OUI_API Window* getWindow(const std::string& name);
            OUI_API Window* addWindow(Window* window);
            OUI_API bool removeWindow(Window* window);
            OUI_API bool deleteWindow(const std::string& name);

        protected:
        
            std::vector<Window*> windows;

            long long lastEvent;

            virtual bool captureEvents();
            virtual void handleEventsForWindow(Window* window);

    };

}

#endif