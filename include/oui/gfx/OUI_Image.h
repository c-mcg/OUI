#ifndef OUI_IMAGE_H
#define OUI_IMAGE_H

#include "OUI_Export.h"

#include <vector>
#include <unordered_map>

struct SDL_Texture;
struct SDL_Renderer;

namespace oui {

	class Window;

	class Image {
		private: static std::unordered_map<Window*, std::unordered_map<std::u16string, Image*>> cachedImages;
		public: OUI_API static Image* loadImage(std::u16string path, Window* window);

		private: int width;
		private: int height;

		private: SDL_Texture* baseImage;

		public: ~Image();

		protected: Image(SDL_Texture* baseImage);

		public: SDL_Texture* getBaseImage();

		public: OUI_API int getWidth();
		public: OUI_API int getHeight();
	
	};

}
#endif
