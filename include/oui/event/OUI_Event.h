#ifndef OUI_EVENT_H
#define OUI_EVENT_H

#include "OUI_Export.h"

namespace oui {

    class OUI_API Event {

        public: static const int MOUSE_IN = 0;
        public: static const int MOUSE_OUT = 1;
        public: static const int MOUSE_DOWN = 2;
        public: static const int MOUSE_UP = 3;
        public: static const int MOUSE_MOVE = 4;
        public: static const int KEY_DOWN = 5;
        public: static const int KEY_UP = 6;
        public: static const int KEY_TYPED = 7;
        public: static const int CLICKED = 8;
        public: static const int MOUSE_WHEEL = 9;
        public: static const int MENU_CLICKED = 10;
        public: static const int WINDOW_CLOSED = 11;
        public: static const int WINDOW_MINIMIZED = 12;
        public: static const int WINDOW_MAXIMIZED = 13;

        public: char type;

        public: bool defaultPrevented;

        public: explicit Event(char type);

        public: virtual bool isMouseEvent();
        public: virtual bool isKeyEvent();
        public: virtual bool isWindowEvent();
        public: bool isScrollEvent();
        public: bool isMenuEvent();

        public: void preventDefault();

    };

    class MouseEvent : public Event {

        private: int x, y;
        private: int globalX, globalY;
        private: int button;

        public: OUI_API MouseEvent(char type, int x, int y, int globalX, int globalY, int button=-1);

        public: OUI_API int getX();
        public: OUI_API int getY();

        public: OUI_API int getGlobalX();
        public: OUI_API int getGlobalY();
        
        public: OUI_API int getButton();

        public: OUI_API bool isMouseEvent() override;

    };

    class KeyEvent : public Event {
        private: int keyCode;
        private: char keyChar;

        public: OUI_API KeyEvent(char type, int keyCode, char keyChar);

        public: OUI_API bool isKeyEvent() override;

        public: OUI_API int getKeyCode();
        public: OUI_API char getKeyChar();
    };

    class ScrollEvent : public Event {
        private: int scroll;

        public: OUI_API explicit ScrollEvent(int scroll);

        public: OUI_API int getScroll();
    };

    class MenuEvent : public Event {
        private: int index;
        private: std::u16string option;

        public: OUI_API MenuEvent(int index, const std::u16string& option);

        public: OUI_API int getIndex();
        public: OUI_API std::u16string getOption();
    };

    class WindowEvent : public Event {

        public: OUI_API explicit WindowEvent(char type);
        public: OUI_API bool isWindowEvent() override;

    };

}
#endif