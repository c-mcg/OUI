
#include "oui/gfx/OUI_Graphics.h"
#include "oui/OUI_Window.h"
#include "util/OUI_StringUtil.h"

#include <iostream>

oui::Graphics::~Graphics() {
    font = NULL;
}

oui::Graphics::Graphics(int width, int height) {
    this->width = width;
    this->height = height;
    this->alpha = 255;
    this->font = NULL;
}

void oui::Graphics::setSize(int width, int height) {
    this->width = width;
    this->height = height;
}

int oui::Graphics::getWidth() {
    return width;
}
int oui::Graphics::getHeight() {
    return height;
}

void oui::Graphics::setColor(Color color) {
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

oui::Image* oui::Graphics::loadImage(const std::u16string& path, Window* window) {
    return NULL;
}
oui::Font* oui::Graphics::loadFont(const std::u16string& name, int size, Window* window) {
    return NULL;
}

void oui::Graphics::clear() {
}

void oui::Graphics::drawRect(int x, int y, int width, int height) {
}

void oui::Graphics::fillRect(int x, int y, int width, int height) {
}

void oui::Graphics::drawLine(int x1, int y1, int x2, int y2) {
}

void oui::Graphics::fillVerticalGradient(int x, int y, int w, int h, oui::Color color1, oui::Color color2) {


}

void oui::Graphics::drawImage(Image* image, int x, int y, int width, int height) {
}

void oui::Graphics::drawImage(Image* image, int x, int y) {
}

    

void oui::Graphics::drawText(const std::u16string& text, int x, int y) {
}

void oui::Graphics::drawTextLine(const std::u16string& text, int x, int y) {
}

void oui::Graphics::setAlpha(int alpha) {
    this->alpha = alpha;
}

int oui::Graphics::getAlpha() {
    return alpha;
}

void oui::Graphics::renderToGraphics(int x, int y, Graphics* target) {
}

void oui::Graphics::renderToWindow(int x, int y) {
}

