#ifndef OUI_FONT_H
#define OUI_FONT_H

#include "OUI_Export.h"
#include <vector>
#include <map>
#include <unordered_map>

namespace oui {

    class Window;

	class Font {

		private: static std::unordered_map<Window*, std::vector<std::u16string>> fontNames;
		private: static std::unordered_map<Window*, std::vector<int>> fontSizes;
		private: static std::unordered_map<Window*, std::vector<Font*>> cachedFonts;

		//Use this function to get a font instance
		//Loads from cache if the font has been loaded already
		public: static OUI_API Font* getFont(const std::u16string& name, int size, Window* window);

		protected: std::u16string name;
		protected: int size;
		protected: Font();

		public: virtual int getStringHeight(const std::u16string& string);
		public: virtual int getStringWidth(const std::u16string& string);

	};

}

#endif
