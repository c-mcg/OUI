#ifndef OUI_LABEL_ATTRIBUTE_MANAGER_H
#define OUI_LABEL_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "components/OUI_ComponentAttributeManager.h"

namespace oui {

    class Font;

    class OUI_API LabelAttributeManager : public ComponentAttributeManager {
    
        public:

            LabelAttributeManager();

            virtual void refreshProfile() override;
            
            Color getTextColor();
            Font* getFont();
            std::u16string getText();
            bool shouldAutoSize();

        private:

            std::u16string text;
            Font* font;
            Color textColor;
            bool autoSize;

    };

}

#endif