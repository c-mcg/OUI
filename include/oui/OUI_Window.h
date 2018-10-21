#ifndef OUI_WINDOW_H
#define OUI_WINDOW_H

#include "OUI_Export.h"
#include "oui/gfx/OUI_Graphics.h"
#include "oui/comp/OUI_Container.h"
#include "oui/event/OUI_Event.h"
#include "oui/comp/OUI_Button.h"
#include "oui/comp/OUI_Panel.h"
#include "oui/comp/OUI_Label.h"
#include "oui/event/OUI_EditEvent.h"
#include <stdint.h>


struct SDL_Window;
struct SDL_Cursor;
struct SDL_Renderer;
union SDL_Event;

namespace oui {

	class Context;

	struct QueuedEvent {
		long long queueTime;
		int delay;
		std::function<void()> func;
	};

	class OUI_API Window: public Container {

#ifdef _DEBUG

		private: oui::Context* context;

#endif

		//TODO: ? Don't export these
		private: SDL_Window* baseWindow;
		public: int baseWindowID;
		public: SDL_Renderer* renderer;

		//Basic window properties
		private: std::u16string title;
		private: bool visible;
		private: char cursorType;
		private: int globalMouseX;
		private: int globalMouseY;
		private: bool maximized;
		private: bool quit;
		private: int maximizeX;
		private: int maximizeY;
		private: int maximizeWidth;
		private: int maximizeHeight;
		private: bool focused;
		private: int editIndex;
		private: std::vector<EditEvent*> editEvents;
		private: std::vector<QueuedEvent*> queuedEvents;
				  

		 //Active window properties
		private: bool moving;
		private: int moveOffX;
		private: int moveOffY;
		private: bool resizing;
		private: int resizeX;
		private: int resizeY;

		//Custimizations
		private: int cursor;
		private: SDL_Cursor* baseCursor;

		private: bool shiftDown;
		private: bool ctrlDown;
		private: bool altDown;

		private: oui::Component* selectedComponent;
		private: oui::Component* rightClickedComponent;

		public: ~Window();
		public: Window();

		

		public: void setContext(Context* context);
		public: Context* getContext();

		public: virtual void setProfile(std::u16string profile);

		public: virtual bool isWindow();

		public: virtual int process();
		public: void handleSDLEvent(SDL_Event* e);//This should not be exported
		public: oui::Graphics* getGraphics();

		public: virtual void handleEvent(Event e);
		public: virtual void redraw();

		public: void setTimeout(int delay, std::function<void()> func);

		public: void setTitle(std::u16string title);
		public: std::u16string getTitle();
		public: void setCursor(int cursor);
		public: bool hasFocus();

		public: void addEditEvent(EditEvent* e, bool append = false);
		public: void undo();
		public: void redo();
		
		public:bool isShiftDown();
		public: bool isCtrlDown();
		public: bool isAltDown();

		public: void setSelectedComponent(Component* component);
		public: Component* getSelectedComponent();
		public: void setRightClickedComponent(Component* component);
		public: Component* getRightClickedComponent();

		public: void close();
		public: void maximize();
		public: void minimize();

		public: void setVisible(bool visible);

		//TODO move to own file (with other functions, sleep, etc.)
		public: void setClipboardText(std::u16string text);
		public: std::u16string getClipboardText();
		public: bool hasClipboardText();

		public: virtual Window* getWindow();
	};

}

#endif