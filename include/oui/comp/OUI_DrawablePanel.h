#ifndef OUI_DRAWABLEPANEL_H
#define OUI_DRAWABLEPANEL_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Container.h"

namespace oui {

	class OUI_API DrawablePanel : public Container {

		private: std::function<void(Graphics*, Component*)> draw;

		public: ~DrawablePanel();

		public: DrawablePanel(const std::string& name, const std::string& classes);

		public: void setDraw(std::function<void(Graphics*, Component*)> draw);

		public: void redraw() override;

	};
}
#endif
