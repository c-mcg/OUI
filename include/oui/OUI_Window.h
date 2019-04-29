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

namespace oui {

    class Context;

    struct QueuedEvent {
        long long queueTime;
        int delay;
        std::function<void()> func;
    };

    class OUI_API Window: public Container {
        

        private: Context* context;

        private: bool shutdown;

        //Basic window properties
        private: std::u16string title;
        private: bool visible;
        private: char cursorType;
        protected: int globalMouseX;
        protected: int globalMouseY;
        private: bool maximized;
        private: int maximizeX;
        private: int maximizeY;
        private: int maximizeWidth;
        private: int maximizeHeight;
        protected: bool focused;
        private: int editIndex;
        private: std::vector<EditEvent*> editEvents;
        private: std::vector<QueuedEvent*> queuedEvents;
                  

         //Active window properties
        protected: bool moving;
        private: int moveOffX;
        private: int moveOffY;
        protected: bool resizing;
        protected: int resizeX;
        protected: int resizeY;

        //Custimizations
        protected: int cursor;

        protected: bool shiftDown;
        protected: bool ctrlDown;
        protected: bool altDown;

        private: Component* selectedComponent;
        private: Component* rightClickedComponent;

        public: ~Window();
        public: Window(int width=0, int height=0);

        protected: void initializeWindow(int width, int height);

        public: void setContext(Context* context);
        public: Context* getContext();

        public: virtual void setProfile(const std::u16string& profile) override;

        public: virtual bool isWindow() override;

        public: virtual int process() override;
        public: Graphics* getGraphics();

        public: void handleMouseMoveEvent(MouseEvent event);
        public: void handleMouseDownEvent(MouseEvent event);
        public: void handleMouseUpEvent(MouseEvent event);
        public: virtual void handleEvent(Event e);
        public: virtual void redraw() override;

        public: void setTimeout(int delay, std::function<void()> func);

        public: std::u16string getTitle();
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


        //TODO move to own file (with other functions, sleep, etc.)
        public: void setClipboardText(const std::u16string& text);
        public: std::u16string getClipboardText();
        public: bool hasClipboardText();

        public: void close();
        public: void setVisible(bool visible);
        public: virtual Window* getWindow() override;

        // Abstract methods
        public: virtual Graphics* createGraphics(int width, int height);

        public: virtual void render();
        public: virtual void showWindow();
        public: virtual void hideWindow();
        public: virtual void setTitle(const std::u16string& title);
        public: virtual void setSize(int width, int height);
        public: virtual void setPosition(int x, int y);
        public: virtual bool setCursor(int cursor);
        public: virtual void minimize();
        public: virtual void maximize();
    };

}

#endif