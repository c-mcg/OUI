#ifndef OUI_FONT_H
#define OUI_FONT_H

#include "OUI_Export.h"
#include <vector>
#include <map>

typedef struct _TTF_Font TTF_Font;

namespace oui {

	class Font {

		private: static std::vector<std::u16string> fontNames;
		private: static std::vector<int> fontSizes;
		private: static std::vector<Font*> cachedFonts;

		//Use this function to get a font instance
		//Loads from cache if the font has been loaded already
		public: static OUI_API Font* getFont(std::u16string name, int size);
		private: static Font* loadFont(std::u16string name, int size);

		private: std::u16string name;
		private: int size;
		private: TTF_Font* baseFont;

		private: Font();

		public: TTF_Font* getBaseFont();

		//This method should not be run from another thread
		public: OUI_API int getStringHeight(std::u16string string);
		//This method should not be run from another thread
		public: OUI_API int getStringWidth(std::u16string string);

	};

}

#endif
