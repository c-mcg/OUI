#ifndef OUI_BUTTON_H
#define OUI_BUTTON_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Component.h"

namespace oui {

    class OUI_API Button : public Component {

        public: static Style* defaultStyle;
        public: static Style* getDefaultButtonStyle();

        private: std::u16string imageString;
        private: Image* image;
        private: bool loadImg;
        private: Color textColor;
        private: Font* font;
        private: bool centerText;

        private: std::u16string text;

        public: ~Button();
        public: Button(const std::string& name, const std::string& classes);

        public: void setProfile(const std::u16string& profile) override;

        public: void redraw() override;

        public: void addedToContainer(Container* container) override;

        public: void setImage(Image* image);
        public: Image* getImage();

        public: Style* getDefaultStyle() override;

    };
}
#endif