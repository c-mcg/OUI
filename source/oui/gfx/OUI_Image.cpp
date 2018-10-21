
#include "oui/gfx/OUI_Image.h"
#include <SDL_image.h>
#include <iostream>
#include "oui/OUI_Window.h"
#include "util/OUI_StringUtil.h"

std::unordered_map<oui::Window*, std::unordered_map<std::u16string, oui::Image*>> oui::Image::cachedImages = std::unordered_map<oui::Window*, std::unordered_map<std::u16string, oui::Image*>>();

oui::Image::~Image() {
	SDL_DestroyTexture(baseImage);
}

oui::Image::Image(SDL_Texture* baseImage) {
	this->baseImage = baseImage;
}

oui::Image* oui::Image::loadImage(std::u16string path, Window* window) {

	std::unordered_map<oui::Window*, std::unordered_map<std::u16string, oui::Image*>>::iterator it = cachedImages.find(window);
	if (it != cachedImages.end()) {
		std::unordered_map<std::u16string, oui::Image*> imgs = it->second;
		std::unordered_map<std::u16string, oui::Image*>::iterator it2;
		if (imgs.find(path) != imgs.end()) {
			return it2->second;
		}
	} else {
		std::unordered_map<std::u16string, oui::Image*> imgs;
		cachedImages.insert({window, imgs});
	}

	SDL_Surface* loadedSurface = IMG_Load(convertUTF16toUTF8(path).c_str());
	if(loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {
		SDL_Texture* texture = SDL_CreateTextureFromSurface(window->getGraphics()->getRenderer(), loadedSurface);
		if(texture == NULL) {
			return NULL;
		}
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		Image* img = new Image(texture);
		
		int w, h;
		SDL_QueryTexture(img->baseImage, NULL, NULL, &w, &h);
		img->width = w;
		img->height = h;
		//std::cout << "img=" << path.c_str() << " imgw=" << img->width << " imgh=" << img->height << std::endl;
		SDL_FreeSurface(loadedSurface);
		std::unordered_map<oui::Window*, std::unordered_map<std::u16string, oui::Image*>>::iterator it = cachedImages.find(window);
		if (it != cachedImages.end()) {
			std::unordered_map<std::u16string, oui::Image*> imgs = it->second;
			imgs.insert({path, img});
		}
		return img;
	}
	return NULL;
}

SDL_Texture* oui::Image::getBaseImage() {
	return baseImage;
}

int oui::Image::getWidth() {
	return width;
}

int oui::Image::getHeight() {
	return height;
}