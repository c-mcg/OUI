#ifndef OUI_TEXT_FIELD_ATTRIBUTE_MANAGER_H
#define OUI_TEXT_FIELD_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "component/OUI_ComponentAttributeManager.h"

namespace oui {

    class Font;

    class OUI_API TextFieldAttributeManager : public ComponentAttributeManager {
    
        public:

            TextFieldAttributeManager();

            Color getTextColor();
            Font* getFont();
            Color getCaratColor();
            int getCaratWidth();
            int getCaratHeightOffset();
            Color getHighlightColor();
            std::u16string getText();

        private:

            Color textColor;
            Font* font;
            Color caratColor;
            int caratWidth;
            int caratHeightOffset;
            Color highlightColor;
            std::u16string text;

    };

}

#endif