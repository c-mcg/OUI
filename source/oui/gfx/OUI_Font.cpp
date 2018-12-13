
#include "oui/gfx/OUI_Font.h"
#include <iostream>
#include "oui/OUI_Window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/OUI_StringUtil.h"

std::unordered_map<oui::Window*, std::vector<oui::Font*>> oui::Font::cachedFonts = std::unordered_map<Window*, std::vector<oui::Font*>>();
std::unordered_map<oui::Window*, std::vector<std::u16string>> oui::Font::fontNames = std::unordered_map<Window*, std::vector<std::u16string>>();
std::unordered_map<oui::Window*, std::vector<int>> oui::Font::fontSizes = std::unordered_map<Window*, std::vector<int>>();

oui::Font::Font() {
    size = 0;
}

oui::Font* oui::Font::getFont(const std::u16string& name, int size, Window* window) {
    auto nameIt = fontNames.find(window);
    auto sizeIt = fontSizes.find(window);
    auto fontIt = cachedFonts.find(window);

    bool newWindow = nameIt != fontNames.end() || sizeIt != fontSizes.end() || fontIt != cachedFonts.end();

    std::vector<std::u16string> fontNameVec = newWindow ? nameIt->second : std::vector<std::u16string>();
    std::vector<int> fontSizeVec = newWindow ? sizeIt->second : std::vector<int>();
    std::vector<oui::Font*> fontVec = newWindow ? fontIt->second : std::vector<Font*>();

    if (!newWindow) {
        for (unsigned int i = 0; i < fontNameVec.size(); i++) {
            std::u16string fontName = fontNameVec.at(i);
            int fontSize = fontSizeVec.at(i);
            if (fontName == name && fontSize == size) {
                return fontVec.at(i);
            }
        }
    } else {
        fontNames.insert({window, fontNameVec});
        fontSizes.insert({window, fontSizeVec});
        cachedFonts.insert({window, fontVec});
    }

	Font* font = window->getGraphics()->loadFont(name, size, window);
	fontNameVec.push_back(name);
	fontSizeVec.push_back(size);
	fontVec.push_back(font);

	return font;
}

//TODO font sizes shoul support utf16
int oui::Font::getStringHeight(const std::u16string& string) {
	return 0;
}
int oui::Font::getStringWidth(const std::u16string& string) {
	return 0;
}