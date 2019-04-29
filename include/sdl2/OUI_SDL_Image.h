#ifndef OUI_SDL_IMAGE_H
#define OUI_SDL_IMAGE_H

#include "oui/gfx/OUI_Image.h"


struct SDL_Texture;
struct SDL_Renderer;

namespace oui {

    class SDLImage : public Image {
        public: OUI_API static Image* loadImage(const std::u16string& path, Window* window);

        private: SDL_Texture* baseImage;

        public: ~SDLImage();

        public: explicit SDLImage(SDL_Texture* baseImage);

        public: SDL_Texture* getBaseImage();
    
    };

}
#endif
