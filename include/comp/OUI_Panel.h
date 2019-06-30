#ifndef OUI_PANEL_H
#define OUI_PANEL_H

#include "OUI_Export.h"
#include "comp/OUI_Container.h"

namespace oui {

    class Panel : public Container {

        public: OUI_API ~Panel();
        public: OUI_API Panel(const std::string& name, const std::string& classes);

    };
}
#endif
