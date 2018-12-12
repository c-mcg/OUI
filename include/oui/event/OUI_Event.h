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

		public: Event(char type);

		public: virtual bool isMouseEvent();
		public: virtual bool isKeyEvent();
		public: virtual bool isWindowEvent();
		public: bool isScrollEvent();
		public: bool isMenuEvent();

		public: void preventDefault();

	};

	class OUI_API MouseEvent : public Event {

		private: int x, y;
        private: int globalX, globalY;
        private: int button;

		public: MouseEvent(char type, int x, int y, int globalX, int globalY, int button=-1);

		public: int getX();
		public: int getY();

        public: int getGlobalX();
		public: int getGlobalY();
		
        public: int getButton();

		public: bool isMouseEvent();

	};

	class OUI_API KeyEvent : public Event {
		private: int keyCode;
		private: char keyChar;

		public: KeyEvent(char type, int keyCode, char keyChar);

		public: bool isKeyEvent();

		public: int getKeyCode();
		public: char getKeyChar();
	};

	class OUI_API ScrollEvent : public Event {
		private: int scroll;

		public: ScrollEvent(int scroll);

		public: int getScroll();
	};

	class OUI_API MenuEvent : public Event {
		private: int index;
		private: std::u16string option;

		public: MenuEvent(int index, std::u16string option);

		public: int getIndex();
		public: std::u16string getOption();
	};

	class OUI_API WindowEvent : public Event {

		public: WindowEvent(char type);
		public: bool isWindowEvent();

	};

}
#endif