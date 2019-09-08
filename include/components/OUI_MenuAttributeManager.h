#ifndef OUI_MENU_ATTRIBUTE_MANAGER_H
#define OUI_MENU_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "components/OUI_ContainerAttributeManager.h"

namespace oui {

    class Font;
    class Button;

    class OUI_API MenuAttributeManager : public ContainerAttributeManager {
    
        public:

            MenuAttributeManager();

            virtual void refreshProfile() override;
            
            int getNumOptions();//TODO this is harder than it looks to remove
            std::vector<std::u16string> getOptions();
        
            std::u16string getFontName();
            int getFontSize();
            int getMinWidth();
            int getOptionHeight();
            Color getHoverColor();
            int getPadding();

        private:

            std::vector<oui::Button*> setOptions(const std::vector<std::u16string>& options);

            int numOptions;//TODO this is harder than it looks to remove
            std::vector<std::u16string> options;
        
            std::u16string fontName;
            int fontSize;
            int minWidth;
            int optionHeight;
            Color hoverColor;
            int padding;

    };

}

#endif