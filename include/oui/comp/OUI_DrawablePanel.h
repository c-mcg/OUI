#ifndef OUI_DRAWABLEPANEL_H
#define OUI_DRAWABLEPANEL_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Container.h"

namespace oui {

	class OUI_API DrawablePanel : public Container {

		private: std::function<void(oui::Graphics*, Component*)> draw;

		public: ~DrawablePanel();

		public: DrawablePanel(std::string name, std::string classes);

		public: void setDraw(std::function<void(oui::Graphics*, Component*)> draw);

		public: virtual void redraw();

	};
}
#endif
