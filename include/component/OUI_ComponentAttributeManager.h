#ifndef OUI_COMPONENT_ATTRIBUTE_MANAGER_H
#define OUI_COMPONENT_ATTRIBUTE_MANAGER_H

#include "OUI_Export.h"
#include "component/attribute/OUI_AttributeManager.h"
#include "gfx/OUI_Color.h"

namespace oui {

    class OUI_API ComponentAttributeManager : public AttributeManager {
    
        public:

            ComponentAttributeManager(Style* defaultStyle = NULL);

            virtual void refreshProfile();
            
            // Behaviour
            bool isVisible();
            bool isInteractable();
            bool isPermanent();

            // Styling
            int getOpacity();
            Color getBackgroundColor1();
            Color getBackgroundColor2();
            std::u16string getCursor();

            std::u16string getBorderStyle();
            int getBorderWidth();
            Color getBorderColor();

            bool getCenteredX();
            bool getCenteredY();

            int getXPercent();
            int getYPercent();
            int getXOffset();
            int getYOffset();
            int getZ();

            // Size
            int getMinWidth();
            int getMinHeight();
            int getWidthPercent();
            int getHeightPercent();
            int getWidthOffset();
            int getHeightOffset();

            virtual void updateAttributeVariable(const std::string& attributeName, Attribute value) override;

            std::vector<std::u16string> getRightClickOptions();

        private:

            // Behaviour
            bool visible;
            bool interactable;
            bool permanent;

            // Styling
            int opacity;
            Color backgroundColor1;
            Color backgroundColor2;
            std::u16string cursor;

            std::u16string borderStyle;
            int borderWidth;
            Color borderColor;

            bool centeredX;
            bool centeredY;

            // Location
            // TODO move to location manager
            int xPercent;
            int yPercent;
            int xOffset;
            int yOffset;
            int z;

            // Size
            int minWidth;
            int minHeight;
            int widthPercent;
            int heightPercent;
            int widthOffset;
            int heightOffset;

            std::vector<std::u16string> rightClickOptions;

    };

}

#endif