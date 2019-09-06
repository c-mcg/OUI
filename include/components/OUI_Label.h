#ifndef OUI_LABEL_H
#define OUI_LABEL_H

#include "OUI_Export.h"
#include "components/OUI_Component.h"
#include "components/OUI_LabelAttributeManager.h"

namespace oui {

    class OUI_API Label : public Component {

        public:
        
            ~Label();
            Label(const std::string& name, const std::string& classes,  EventDispatcher* eventDispatcher=new EventDispatcher(), LabelAttributeManager* attributeManager=new LabelAttributeManager());

            void redraw() override;

        private:

            LabelAttributeManager* getAttributeManager();

    };
}
#endif
