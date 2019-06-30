#ifndef OUI_BUTTON_H
#define OUI_BUTTON_H

#include "OUI_Export.h"
#include "comp/OUI_Component.h"

namespace oui {

    class Button : public Component {

        public: static Style* defaultStyle;
        public: static Style* getDefaultButtonStyle();

        private: std::u16string link;
        private: std::u16string imageString;
        private: Image* image;
        private: Color textColor;
        private: Font* font;
        private: bool centerText;

        private: std::u16string text;

        public: OUI_API ~Button();
        public: OUI_API Button(const std::string& name, const std::string& classes);

        public: OUI_API void setProfile(const std::u16string& profile) override;

        public: OUI_API void redraw() override;

        public: OUI_API void addedToContainer(Container* container) override;

        public: OUI_API void setImage(Image* image);
        public: OUI_API Image* getImage();

        public: OUI_API Style* getDefaultStyle() override;

    };
}
#endif