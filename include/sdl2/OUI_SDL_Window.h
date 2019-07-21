#ifndef OUI_SDL_WINDOW_H
#define OUI_SDL_WINDOW_H

#include "OUI_Export.h"
#include "OUI_Window.h"


struct SDL_Window;
struct SDL_Cursor;
struct SDL_Renderer;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
union SDL_Event;

namespace oui {

    class SDLWindow : public Window {

        public: 
        
            int baseWindowId;

            ~SDLWindow();
            SDLWindow(int width=0, int height=0);

            void handleSDLEvent(SDL_Event* event);

            Graphics* createGraphics(int width, int height) override;
            
            void render() override;
            
            void showWindow() override;
            void hideWindow() override;

            void setTitle(const std::u16string& title) override;
            void setSize(int width, int height) override;
            void setPosition(int x, int y) override;
            bool setCursor(int cursor) override;
            
            void onMinimize(ComponentEvent* event) override;
            void onMaximize(ComponentEvent* event) override;

            virtual bool isAltDown();
            virtual bool isCtrlDown();
            virtual bool isMetaDown();
            virtual bool isShiftDown();


        protected: 
        
            SDL_Window* baseWindow;
            SDL_Renderer* renderer;
            SDL_Cursor* baseCursor;

            bool altDown;
            bool ctrlDown;
            bool metaDown;
            bool shiftDown;
            int lastMouseX;
            int lastMouseY;

            std::vector<int> mouseButtonsDown;


    };

}

#endif