#ifndef OUI_SDL_WINDOW_H
#define OUI_SDL_WINDOW_H

#include "OUI_Export.h"
#include "oui/OUI_Window.h"


struct SDL_Window;
struct SDL_Cursor;
struct SDL_Renderer;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
union SDL_Event;

namespace oui {

    class SDLWindow : public Window {

        public: int baseWindowId;
		protected: SDL_Window* baseWindow;
		protected: SDL_Renderer* renderer;

		protected: SDL_Cursor* baseCursor;

        public: ~SDLWindow();
        public: SDLWindow(int width=0, int height=0);

        public: void handleSDLEvent(SDL_Event* event);
        
        public: virtual Graphics* createGraphics(int width, int height);

        public: virtual void render();
        public: virtual void showWindow();
        public: virtual void hideWindow();

        public: virtual void setTitle(std::u16string title);
        public: virtual void setSize(int width, int height);
        public: virtual void setPosition(int x, int y);
        public: virtual bool setCursor(int cursor);
        public: virtual void minimize();
        public: virtual void maximize();

    };

}

#endif