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
		public: Button(std::string name, std::string classes);

		public: virtual void setProfile(std::u16string profile);

		public: virtual void redraw();

		public: virtual void addedToContainer(Container* container);

		public: void setImage(Image* image);
		public: Image* getImage();

		public: virtual Style* getDefaultStyle();

	};
}
#endif