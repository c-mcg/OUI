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
        
        public: Graphics* createGraphics(int width, int height) override;

        public: void render() override;
        public: void showWindow() override;
        public: void hideWindow() override;

        public: void setTitle(const std::u16string& title) override;
        public: void setSize(int width, int height) override;
        public: void setPosition(int x, int y) override;
        public: bool setCursor(int cursor) override;
        public: void minimize() override;
        public: void maximize() override;

    };

}

#endif