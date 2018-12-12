#ifndef OUI_SCROLLPANEL_H
#define OUI_SCROLLPANEL_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Container.h"

namespace oui {

	class OUI_API ScrollPanel : public Container {

		public: static const int SCROLLBAR_SIZE = 20;//TODO make this into an attribute
		public: static const int BAR_PADDING_WIDTH = 2;//This is the space around the width of the bar
		public: static const int BAR_PADDING_HEIGHT = 1;//This is the space at the tips of the bar

		public: static const int SCROLL_WHEEL_SPEED = 10;//100 for scroll wheel previously
		public: static const int SCROLL_BUTTON_SPEED = 40;

		private: bool scrollingX, scrollingY;
		private: bool resetX, resetY;
		private: int scrollPosX, scrollPosY;
		private: int clickStartX, clickStartY;
		private: int childWidth, childHeight;

		private: bool childrenChanged;

		public: ~ScrollPanel();
		public: ScrollPanel(std::string name, std::string classes);

		private: void createScrollBar(bool vertical);
		private: void getChildSize(int* w, int* h);//TODO move this to container class for autosizing (TODO padding) (this will probably be great for scrollbars)
		private: bool addScrollBar(Container* scrollbar);

		public: virtual bool addChild(Component* child);

		public: virtual Component* removeChild(int index, bool shouldDelete = false);
		public: virtual void removeAllChildren(bool shouldDelete = false);

		public: virtual void setProfile(std::u16string profile);
		public: virtual void handleEvent(Event& e);
		public: virtual int process();
		public: virtual void redraw();
		private: virtual void resize();

		public: void scrollX(int speed);
		public: void scrollY(int speed);

	};
}
#endif