#ifndef OUI_LABEL_H
#define OUI_LABEL_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Component.h"

namespace oui {

	class OUI_API Label : public Component {

		private: std::u16string text;
		private: Font* font;
		private: Color textColor;
		private: bool autoSize;

		public: ~Label();
		public: Label(std::string name, std::string classes);

		public: virtual void setProfile(std::u16string profile);
		public: virtual void redraw();

	};
}
#endif
