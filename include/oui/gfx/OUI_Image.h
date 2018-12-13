#ifndef OUI_IMAGE_H
#define OUI_IMAGE_H

#include "OUI_Export.h"

#include <vector>
#include <unordered_map>

namespace oui {

	class Window;

	class OUI_API Image {
		private: static std::unordered_map<Window*, std::unordered_map<std::u16string, Image*>> cachedImages;
		public: static Image* loadImage(const std::u16string& path, Window* window);
		public: static void cacheImage(Window* window, std::u16string path, Image* image);

		protected: int width;
		protected: int height;

		public: ~Image();

		protected: Image();
        
		public: int getWidth();
		public: int getHeight();
	
	};

}
#endif
