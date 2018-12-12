
#include "oui/gfx/OUI_Image.h"
#include <iostream>
#include "oui/OUI_Window.h"
#include "util/OUI_StringUtil.h"

std::unordered_map<oui::Window*, std::unordered_map<std::u16string, oui::Image*>> oui::Image::cachedImages = std::unordered_map<oui::Window*, std::unordered_map<std::u16string, oui::Image*>>();

oui::Image::~Image() {
}

oui::Image::Image() {
}

oui::Image* oui::Image::loadImage(std::u16string path, Window* window) {

	auto it = cachedImages.find(window);
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

	return window->getGraphics()->loadImage(path, window);
}

void oui::Image::cacheImage(Window* window, std::u16string path, Image* image) {
    auto it = cachedImages.find(window);
    
    std::unordered_map<std::u16string, oui::Image*> imgs;
    if (it != cachedImages.end()) {
        imgs = it->second;
    } else {
        imgs = std::unordered_map<std::u16string, oui::Image*>();
    }
    imgs.insert({path, image});
}

int oui::Image::getWidth() {
	return width;
}

int oui::Image::getHeight() {
	return height;
}