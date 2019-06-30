#ifndef OUI_LABEL_H
#define OUI_LABEL_H

#include "OUI_Export.h"
#include "components/OUI_Component.h"

namespace oui {

    class Label : public Component {

        private: std::u16string text;
        private: Font* font;
        private: Color textColor;
        private: bool autoSize;

        public: OUI_API ~Label();
        public: OUI_API Label(const std::string& name, const std::string& classes);

        public: OUI_API void setProfile(const std::u16string& profile) override;
        public: OUI_API void redraw() override;

    };
}
#endif
