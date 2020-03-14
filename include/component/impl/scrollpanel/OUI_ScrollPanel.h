#ifndef OUI_SCROLLPANEL_H
#define OUI_SCROLLPANEL_H

#include "OUI_Export.h"
#include "component/impl/container/OUI_Container.h"
#include "component/impl/scrollpanel/OUI_ScrollPanelAttributeManager.h"

namespace oui {

    class ScrollPanel : public Container {

        public:

            OUI_API static const int SCROLLBAR_SIZE = 20;//TODO make this into an attribute
            OUI_API static const int BAR_PADDING_WIDTH = 2;//This is the space around the width of the bar
            OUI_API static const int BAR_PADDING_HEIGHT = 1;//This is the space at the tips of the bar

            OUI_API static const int SCROLL_WHEEL_SPEED = 10;//100 for scroll wheel previously
            OUI_API static const int SCROLL_BUTTON_SPEED = 40;

            OUI_API ~ScrollPanel();
            OUI_API ScrollPanel(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher=new EventDispatcher(), ScrollPanelAttributeManager* attributeManager=new ScrollPanelAttributeManager());

            OUI_API bool addChild(Component* child) override;

            OUI_API Component* removeChild(int index, bool shouldDelete = false) override;
            OUI_API void removeAllChildren(bool shouldDelete = false) override;

            OUI_API int process() override;
            OUI_API void redraw() override;
            void handleResize();

            OUI_API void scrollX(int speed);
            OUI_API void scrollY(int speed);

            OUI_API virtual ScrollPanelAttributeManager* getAttributeManager() override;

        private:

            void onScroll(ComponentEvent* e);

            bool scrollingX, scrollingY;
            bool resetX, resetY;
            int scrollPosX, scrollPosY;
            int clickStartX, clickStartY;
            int childWidth, childHeight;

            bool childrenChanged;

            void createScrollBar(bool vertical);
            void getChildSize(int* w, int* h);//TODO move this to container class for autosizing (TODO padding) (this will probably be great for scrollbars)
            bool addScrollBar(Container* scrollbar);

    };
}
#endif