#ifndef OUI_SDL_FONT_H
#define OUI_SDL_FONT_H

#include "oui/gfx/OUI_Font.h"

#include <unordered_map>

typedef struct _TTF_Font TTF_Font;

namespace oui {

    class Window;

	class SDLFont : public Font {

		private: static Font* loadFont(std::u16string name, int size);

		private: TTF_Font* baseFont;

		public: SDLFont(std::u16string name, int size, TTF_Font* baseFont);

		public: TTF_Font* getBaseFont();

        //This method should not be run from another thread
		public: virtual int getStringHeight(std::u16string string);
		//This method should not be run from another thread
		public: virtual int getStringWidth(std::u16string string);


	};

}

#endif
