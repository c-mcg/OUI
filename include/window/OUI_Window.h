#ifndef OUI_WINDOW_H
#define OUI_WINDOW_H

#include "OUI_Export.h"
#include "gfx/OUI_Graphics.h"
#include "component/impl/container/OUI_Container.h"
#include "component/impl/button/OUI_Button.h"
#include "component/impl/panel/OUI_Panel.h"
#include "component/impl/label/OUI_Label.h"
#include "event/OUI_EditEvent.h"
#include <stdint.h>

#include "event/OUI_ComponentEvent.h"
#include "event/OUI_WindowEventDispatcher.h"
#include "window/OUI_WindowAttributeManager.h"
#include "window/OUI_MouseManager.h"

namespace oui {

    class Context;

    struct QueuedEvent {
        long long queueTime;
        int delay;
        std::function<void()> func;
    };

    class Window: public Container {

        public:

            bool isResizing();
            void startResizing();
            void stopResizing();
            bool isMoving();
            void stopMoving();
            virtual int getMouseX() override;
            virtual int getMouseY() override;

            virtual std::vector<int> getMouseButtonsDown();

        protected:

            MouseManager* mouseManager;
        
        private:

            WindowAttributeManager* getAttributeManager();

        private: Context* context;

        private: bool shutdown;

        // Basic window properties
        private: std::u16string title;
        protected: std::u16string cursorType;
        private: bool maximized;
        protected: bool minimized;
        private: int maximizeX;
        private: int maximizeY;
        private: int maximizeWidth;
        private: int maximizeHeight;
        protected: bool focused;
        private: int editIndex;
        private: std::vector<EditEvent*> editEvents;
        private: std::vector<QueuedEvent*> queuedEvents;
                  
        // Active window properties
        protected: bool moving;
        private: int moveOffX;
        private: int moveOffY;
        protected: bool resizing;
        protected: int resizeX;
        protected: int resizeY;

        private: Component* selectedComponent;

        public: OUI_API ~Window();
        public: OUI_API Window(int width=0, int height=0, EventDispatcher* eventDispatcher=new WindowEventDispatcher(), WindowAttributeManager* attributeManager = new WindowAttributeManager(), MouseManager* MouseManager=new MouseManager());

        protected: void initializeWindow(int width, int height);

        public: OUI_API void setContext(Context* context);
        public: OUI_API Context* getContext();

        public: OUI_API virtual bool isWindow() override;

        public: OUI_API virtual int process() override;
        public: OUI_API Graphics* getGraphics();

        public: OUI_API void onMouseDown(ComponentEvent* event);
		public: OUI_API void onKeyDown(ComponentEvent* event);
        
        public: OUI_API virtual void redraw() override;

        public: OUI_API void setTimeout(int delay, std::function<void()> func);

        public: OUI_API std::u16string getTitle();
        public: OUI_API bool hasFocus();

        public: OUI_API void addEditEvent(EditEvent* e, bool append = false);
        public: OUI_API void undo();
        public: OUI_API void redo();
        
        public: OUI_API virtual bool isAltDown();
        public: OUI_API virtual bool isCtrlDown();
        public: OUI_API virtual bool isMetaDown();
        public: OUI_API virtual bool isShiftDown();

        public: OUI_API void setSelectedComponent(Component* component);
        public: OUI_API Component* getSelectedComponent();
        public: OUI_API void closeRightClickMenu();


        //TODO move to own file (with other functions, sleep, etc.)
        public: OUI_API void setClipboardText(const std::u16string& text);
        public: OUI_API std::u16string getClipboardText();
        public: OUI_API bool hasClipboardText();

        public: OUI_API void setVisible(bool visible);
        public: OUI_API void setPage(std::u16string path);
        public: OUI_API virtual Window* getWindow() override;

        // Abstract methods
        public: OUI_API virtual Graphics* createGraphics(int width, int height);

        public: OUI_API virtual void render();
        public: OUI_API virtual void showWindow();
        public: OUI_API virtual void hideWindow();
        public: OUI_API virtual void setTitle(const std::u16string& title);
        public: OUI_API virtual void setSize(int width, int height);
        public: OUI_API virtual void setPosition(int x, int y);
        public: OUI_API virtual bool setCursor(std::u16string cursor);

        public: OUI_API void onSystemKeyDown(ComponentEvent* event);
        public: OUI_API void onSystemKeyUp(ComponentEvent* event);
        public: OUI_API void onSystemKeyTyped(ComponentEvent* event);
        public: OUI_API virtual void onSystemClose(ComponentEvent* event=NULL);
        public: OUI_API virtual void onSystemMinimize(ComponentEvent* event=NULL);
        public: OUI_API virtual void onSystemUnminimize(ComponentEvent* event=NULL);
        public: OUI_API virtual void onSystemMaximize(ComponentEvent* event=NULL);
        public: OUI_API virtual void onSystemUnmaximize(ComponentEvent* event=NULL);
        public: OUI_API virtual void onSystemFocus(ComponentEvent* event=NULL);
        public: OUI_API virtual void onSystemBlur(ComponentEvent* event=NULL);
        
        public: OUI_API virtual void onClose(ComponentEvent* event=NULL);
        public: OUI_API virtual void onMinimize(ComponentEvent* event=NULL);
        public: OUI_API virtual void onUnminimize(ComponentEvent* event=NULL);
        public: OUI_API virtual void onMaximize(ComponentEvent* event=NULL);
        public: OUI_API virtual void onUnmaximize(ComponentEvent* event=NULL);
        public: OUI_API virtual void onFocus(ComponentEvent* event=NULL);
        public: OUI_API virtual void onBlur(ComponentEvent* event=NULL);
    };

}

#endif