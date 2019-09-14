#ifndef OUI_DRAWABLEPANEL_H
#define OUI_DRAWABLEPANEL_H

#include "OUI_Export.h"
#include "components/container/OUI_Container.h"

namespace oui {

    class DrawablePanel : public Container {

        private: std::function<void(Graphics*, Component*)> draw;

        public: OUI_API ~DrawablePanel();

        public: OUI_API DrawablePanel(const std::string& name, const std::string& classes);

        public: OUI_API void setDraw(std::function<void(Graphics*, Component*)> draw);

        public: OUI_API void redraw() override;

    };
}
#endif
