
#include "oui/gfx/OUI_Graphics.h"
#include "util/OUI_StringUtil.h"

#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

oui::Graphics::~Graphics() {
	if (buffer != NULL) {
		SDL_DestroyTexture(buffer);
	}
	buffer = NULL;
	font = NULL;
}

oui::Graphics::Graphics(int width, int height) {
	this->width = width;
	this->height = height;
	this->alpha = 255;
	this->font = Font::getFont(u"notoserif", 12);
	this->renderer = NULL;
	this->buffer = NULL;
	if (width == 0 || height == 0) {

	}
}

void oui::Graphics::setRenderer(SDL_Renderer* renderer) {
	this->renderer = renderer;
	setSize(width, height);
}

SDL_Renderer* oui::Graphics::getRenderer() {
	if(renderer == NULL) {
		return NULL;
	}
	return renderer;
}

void oui::Graphics::setSize(int width, int height) {
	if (width == 0 || height == 0) {
		int i = 5;
	}

	this->width = width;
	this->height = height;
	if (buffer != NULL) {
		SDL_DestroyTexture(buffer);
	}
	this->buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, width, height);
	if (SDL_SetTextureBlendMode(buffer, SDL_BLENDMODE_BLEND) < 0) {
		buffer = NULL;
	}
	if (buffer != NULL) {
		SDL_SetTextureAlphaMod(buffer, alpha);
	}
}

int oui::Graphics::getWidth() {
	return width;
}
int oui::Graphics::getHeight() {
	return height;
}

void oui::Graphics::setColor(oui::Color color) {
	this->currentColor = color;
}

oui::Color oui::Graphics::getColor() {
	return currentColor;
}

void oui::Graphics::setFont(Font* font) {
	this->font = font;
}

oui::Font* oui::Graphics::getFont() {
	return font;
}

void oui::Graphics::clear() {
	prepare();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void oui::Graphics::drawRect(int x, int y, int width, int height) {
	if (currentColor.getA() == 0) {
		return;
	}
	prepare();
	SDL_Rect rect = { x, y, width ,height };
	SDL_RenderDrawRect(renderer, &rect);
}

void oui::Graphics::fillRect(int x, int y, int width, int height) {
	if (currentColor.getA() == 0) {
		return;
	}
	prepare();
	SDL_Rect rect = { x, y, width ,height };
	SDL_RenderFillRect(renderer, &rect);
}

void oui::Graphics::drawLine(int x1, int y1, int x2, int y2) {
	if (currentColor.getA() == 0) {
		return;
	}
	prepare();
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void oui::Graphics::fillVerticalGradient(int x, int y, int w, int h, oui::Color color1, oui::Color color2) {
	if (color1.getA() == 0 && color2.getA() == 0) {
		return;
	}
	prepare();
	int redDiff = color1.getR() - color2.getR();
	int greenDiff = color1.getG() - color2.getG();
	int blueDiff = color1.getB() - color2.getB();
	int alphaDiff = color1.getA() - color2.getA();
	int clr1R = color1.getR();
	int clr1G = color1.getG();
	int clr1B = color1.getB();
	int clr1A = color1.getA();
	for (int i = 0; i < h; i++) {
		float percent = (float)(i + 1) / h;
		SDL_SetRenderDrawColor(renderer, clr1R - (int)(redDiff * percent), clr1G - (int)(greenDiff * percent), clr1B - (int)(blueDiff * percent), clr1A - (int)(alphaDiff * percent));
		SDL_RenderDrawLine(renderer, x, y + i, x + w - 1, y + i);
	}

}

void oui::Graphics::drawImage(Image* image, int x, int y, int width, int height) {
	prepare();
	SDL_Rect dst = {x, y, width, height};
	SDL_RenderCopy(renderer, image->getBaseImage(), NULL, &dst);
}
void oui::Graphics::drawImage(Image* image, int x, int y) {
	prepare();
	SDL_Rect dst = {x, y, image->getWidth(), image->getHeight()};
	SDL_RenderCopy(renderer, image->getBaseImage(), NULL, &dst);
}

	

void oui::Graphics::drawText(std::u16string text, int x, int y) {
	if (text.length() == 0) {
		return;
	}
	if (font == NULL) {
		printf("Can't draw text");
		return;
	}

	SDL_Color foregroundColor = {(Uint8) currentColor.getR(), (Uint8) currentColor.getG(), (Uint8) currentColor.getB()};
	SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(font->getBaseFont(), reinterpret_cast<const Uint16*>(text.c_str()), foregroundColor);
	if (textSurface == NULL) {
		printf("Error rendering font : %s", TTF_GetError());
		std::cout << "Text surface was null!" << std::endl;
		return;
	}
	SDL_Rect textLocation = {x, y, textSurface->w, textSurface->h};
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_SetTextureAlphaMod(texture, (Uint8) currentColor.getA());

	prepare();
	SDL_RenderCopy(renderer, texture, NULL, &textLocation);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(texture);
	//drawText(std::u16string, x, y, 0, stringWidthSingleLine(std::u16string));
}

void oui::Graphics::setAlpha(int alpha) {
	this->alpha = alpha;
	if (buffer != NULL) {
		SDL_SetTextureAlphaMod(buffer, alpha);
	}
}

int oui::Graphics::getAlpha() {
	return alpha;
}

void oui::Graphics::render(int x, int y) {
	SDL_Rect dest = {x, y, width, height};
	//std::cout << "graphics: width=" << width << " height=" << height << std::endl;
	SDL_RenderCopy(renderer, buffer, NULL, &dest);
}

void oui::Graphics::prepare() {
	SDL_SetRenderTarget(renderer, buffer);
	SDL_SetRenderDrawColor(renderer, currentColor.getR(), currentColor.getG(), currentColor.getB(), currentColor.getA());
}