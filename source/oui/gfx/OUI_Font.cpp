
#include "oui/gfx/OUI_Font.h"
#include <SDL_ttf.h>
#include <iostream>
#include "oui/OUI_Window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/OUI_StringUtil.h"

std::vector<oui::Font*> oui::Font::cachedFonts = std::vector<oui::Font*>();
std::vector<std::u16string> oui::Font::fontNames = std::vector<std::u16string>();
std::vector<int> oui::Font::fontSizes = std::vector<int>();

oui::Font::Font() {

}

oui::Font* oui::Font::getFont(std::u16string name, int size) {
	for (unsigned int i = 0; i < fontNames.size(); i++) {
		if (fontNames.at(i) == name && fontSizes.at(i) == size) {
			return cachedFonts.at(i);
		}
	}
	Font* font = loadFont(name, size);
	fontNames.insert(fontNames.end(), name);
	fontSizes.insert(fontSizes.end(), size);
	cachedFonts.insert(cachedFonts.end(), font);

	return font;
}

oui::Font* oui::Font::loadFont(std::u16string name, int size) {
	TTF_Font* mFont = TTF_OpenFont(std::string("data/fonts/" + convertUTF16toUTF8(name) + ".ttf").c_str(), size);
	if (mFont == NULL) {
		std::cout << "Error loading font: " << convertUTF16toUTF8(name).c_str() << " " << TTF_GetError() << std::endl;
		if (name != u"notoserif") {
			return getFont(u"notoserif", size);
		}
		return NULL;
	}
	int w, h;
	TTF_SizeText(mFont, "YO", &w, &h);
	if (h == 0 || w == 0) {
		std::cout << "The font was not properly loaded: name=" << convertUTF16toUTF8(name).c_str() << " size=" << size << std::endl;

	}
	Font* font = new Font();
	font->baseFont = mFont;
	font->name = name;
	font->size = size;
	std::cout << "Successfully loaded font: " << convertUTF16toUTF8(name).c_str() << " " << size << std::endl;
	return font;
}

TTF_Font* oui::Font::getBaseFont() {
	
	return baseFont;
}

//TODO font sizes shoul support utf16
int oui::Font::getStringHeight(std::u16string string) {
	int w, h;
	if (TTF_SizeText(baseFont, convertUTF16toUTF8(string).c_str(), &w, &h) != -1) {
		return h;
	}
	return 0;
}
int oui::Font::getStringWidth(std::u16string string) {
	//std::cout << "Get String width: '" << string.c_str() << "'" << std::endl;
	int w, h;
	if (TTF_SizeText(baseFont, convertUTF16toUTF8(string).c_str(), &w, &h) != -1) {
		return w;
	}
	return 0;
}