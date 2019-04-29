#ifndef OUI_SDL_CONTEXT_H
#define OUI_SDL_CONTEXT_H

#include "OUI_Export.h"
#include "OUI_Context.h"

#include "sdl2/OUI_SDL_Window.h"

namespace oui {

    class SDLContext : public Context {

        private: long long lastSDLEvent;

        public: SDLContext();

        protected: Window* createWindow(int width=0, int height=0);

        protected: bool pollEvents() override;

        private: SDLWindow* getWindow(int sdlID);

    };

}

#endif