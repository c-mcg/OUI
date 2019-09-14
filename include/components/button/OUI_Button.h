#ifndef OUI_BUTTON_H
#define OUI_BUTTON_H

#include "OUI_Export.h"
#include "components/OUI_Component.h"
#include "components/button/OUI_ButtonAttributeManager.h"

namespace oui {

    class Button : public Component {

        public:
        
            static Style* defaultStyle;
            static Style* getDefaultButtonStyle();

            OUI_API ~Button();
            OUI_API Button(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher=new EventDispatcher(), ButtonAttributeManager* attributeManager=new ButtonAttributeManager());

            OUI_API void redraw() override;

            OUI_API void addedToContainer(Container* container) override;

            OUI_API virtual ButtonAttributeManager* getAttributeManager() override;

    };
}
#endif