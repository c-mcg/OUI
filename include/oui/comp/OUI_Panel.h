#ifndef OUI_PANEL_H
#define OUI_PANEL_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Container.h"

namespace oui {

	class OUI_API Panel : public Container {

		public: ~Panel();
		public: Panel(const std::string& name, const std::string& classes);

	};
}
#endif
