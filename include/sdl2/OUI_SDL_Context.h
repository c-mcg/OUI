#ifndef OUI_SDL_CONTEXT_H
#define OUI_SDL_CONTEXT_H

#include "OUI_Export.h"
#include "OUI_Context.h"

#include "sdl2/OUI_SDL_Window.h"

namespace oui {

    class SDLContext : public Context {

        public:

            SDLContext();

        protected:

            Window* createWindow(int width=0, int height=0);

            bool captureEvents() override;
            void handleEventsForWindow(Window* window) override;

        private:

            SDLWindow* getWindow(int sdlID);

            long long lastSDLEvent;
            std::unordered_map<SDLWindow*, std::vector<SDL_Event>> capturedEvents;

    };

}

#endif