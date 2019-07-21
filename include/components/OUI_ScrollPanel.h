#ifndef OUI_SCROLLPANEL_H
#define OUI_SCROLLPANEL_H

#include "OUI_Export.h"
#include "components/OUI_Container.h"

namespace oui {

    class ScrollPanel : public Container {

        public: OUI_API static const int SCROLLBAR_SIZE = 20;//TODO make this into an attribute
        public: OUI_API static const int BAR_PADDING_WIDTH = 2;//This is the space around the width of the bar
        public: OUI_API static const int BAR_PADDING_HEIGHT = 1;//This is the space at the tips of the bar

        public: OUI_API static const int SCROLL_WHEEL_SPEED = 10;//100 for scroll wheel previously
        public: OUI_API static const int SCROLL_BUTTON_SPEED = 40;

        private:
            void onScroll(ComponentEvent* e);

        private: bool scrollingX, scrollingY;
        private: bool resetX, resetY;
        private: int scrollPosX, scrollPosY;
        private: int clickStartX, clickStartY;
        private: int childWidth, childHeight;

        private: bool childrenChanged;

        public: OUI_API ~ScrollPanel();
        public: OUI_API ScrollPanel(const std::string& name, const std::string& classes);

        private: void createScrollBar(bool vertical);
        private: void getChildSize(int* w, int* h);//TODO move this to container class for autosizing (TODO padding) (this will probably be great for scrollbars)
        private: bool addScrollBar(Container* scrollbar);

        public: OUI_API bool addChild(Component* child) override;

        public: OUI_API Component* removeChild(int index, bool shouldDelete = false) override;
        public: OUI_API void removeAllChildren(bool shouldDelete = false) override;

        public: OUI_API void setProfile(const std::u16string& profile) override;
        public: OUI_API int process() override;
        public: OUI_API void redraw() override;
        private: void resize();

        public: OUI_API void scrollX(int speed);
        public: OUI_API void scrollY(int speed);

    };
}
#endif