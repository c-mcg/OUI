#include "sdl2/OUI_SDL_Image.h"
#include <SDL_image.h>
#include <iostream>
#include "util/OUI_StringUtil.h"

oui::SDLImage::~SDLImage() {
    SDL_DestroyTexture(baseImage);
}

oui::SDLImage::SDLImage(SDL_Texture* baseImage) {
    this->baseImage = baseImage;
    SDL_QueryTexture(baseImage, NULL, NULL, &this->width, &this->height);

}

SDL_Texture* oui::SDLImage::getBaseImage() {
    return baseImage;
}