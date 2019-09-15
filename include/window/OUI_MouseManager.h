#ifndef OUI_MOUSE_MANAGER_H
#define OUI_MOUSE_MANAGER_H

#include "OUI_Export.h"

namespace oui {

    class Window;
    class ComponentEvent;

    class OUI_API MouseManager {
    
        public:

            MouseManager();

            void setWindow(Window* window);
            int getGlobalMouseX();
            int getGlobalMouseY();
            int getMouseX();
            int getMouseY();

            void onSystemMouseMove(ComponentEvent* event);
            void onSystemMouseUp(ComponentEvent* event);
            void onSystemMouseDown(ComponentEvent* event);
            void onSystemScrollWheel(ComponentEvent* event);

            /**
             * Called every frame, used to fire global mouse move events
             */
            void process();

        protected:

            int globalMouseX;
            int globalMouseY;
            int mouseX;
            int mouseY;

        private:

            void dispatchEvent(ComponentEvent* event);

            Window* window;
            
    };

}

#endif