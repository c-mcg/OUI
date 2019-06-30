
#include "gfx/OUI_Image.h"
#include <iostream>
#include "OUI_Window.h"
#include "util/OUI_StringUtil.h"

std::unordered_map<oui::Window*, std::unordered_map<std::u16string, oui::Image*>> oui::Image::cachedImages = std::unordered_map<oui::Window*, std::unordered_map<std::u16string, oui::Image*>>();

oui::Image::~Image() {
}

oui::Image::Image() {
    width = 0;
    height = 0;
}

oui::Image* oui::Image::loadImage(const std::u16string& path, Window* window) {

    auto it = cachedImages.find(window);
    if (it != cachedImages.end()) {
        std::unordered_map<std::u16string, oui::Image*> imgs = it->second;
        auto it2 = imgs.find(path);
        if (it2 != imgs.end()) {
            return it2->second;
        }
    } else {
        std::unordered_map<std::u16string, oui::Image*> imgs;
        cachedImages.insert({window, imgs});
    }

    return window->getGraphics()->loadImage(path, window);
}

void oui::Image::cacheImage(Window* window, const std::u16string path, Image* image) {
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