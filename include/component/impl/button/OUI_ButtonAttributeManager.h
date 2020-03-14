#ifndef OUI_BUTTON_ATTRIBUTE_MANAGER_H
#define OUI_BUTTON_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "component/OUI_ComponentAttributeManager.h"

namespace oui {

    class Image;
    class Font;

    class OUI_API ButtonAttributeManager : public ComponentAttributeManager {
    
        public:

            ButtonAttributeManager();

            std::u16string getLink();
            Image* getImage();
            Color getTextColor();
            Font* getFont();
            bool isTextCentered();

            std::u16string getText();

        private:
        
            std::u16string link;
            Image* image;
            Color textColor;
            Font* font;
            bool centerText;

            std::u16string text;

    };

}

#endif