#ifndef OUI_BUTTON_ATTRIBUTE_MANAGER_H
#define OUI_BUTTON_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "components/OUI_ComponentAttributeManager.h"

namespace oui {

    class Image;
    class Font;

    class OUI_API ButtonAttributeManager : public ComponentAttributeManager {
    
        public:

            ButtonAttributeManager();

            virtual void setProfile(const std::u16string& profile) override;
            
            std::u16string getLink();
            std::u16string getImageString();
            Color getTextColor();
            Font* getFont();
            bool isTextCentered();

            std::u16string getText();

        private:
        
            std::u16string link;
            std::u16string imageString;
            Color textColor;
            Font* font;
            bool centerText;

            std::u16string text;

    };

}

#endif