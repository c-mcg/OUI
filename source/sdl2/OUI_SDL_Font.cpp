
#include "sdl2/OUI_SDL_Font.h"
#include <SDL_ttf.h>
#include <iostream>
#include "oui/OUI_Window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/OUI_StringUtil.h"

oui::SDLFont::SDLFont(std::u16string name, int size, TTF_Font* baseFont) {
    this->name = name;
    this->size = size;
    this->baseFont = baseFont;
}

TTF_Font* oui::SDLFont::getBaseFont() {
	return baseFont;
}


//TODO font sizes shoul support utf16
int oui::SDLFont::getStringHeight(std::u16string string) {
	int w, h;
	if (TTF_SizeText(baseFont, convertUTF16toUTF8(string).c_str(), &w, &h) != -1) {
		return h;
	}
	return 0;
}
int oui::SDLFont::getStringWidth(std::u16string string) {
	//std::cout << "Get String width: '" << string.c_str() << "'" << std::endl;
	int w, h;
	if (TTF_SizeText(baseFont, convertUTF16toUTF8(string).c_str(), &w, &h) != -1) {
		return w;
	}
	return 0;
}