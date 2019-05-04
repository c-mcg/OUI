
#include "sdl2/OUI_SDL_Graphics.h"
#include "oui/OUI_Window.h"
#include "sdl2/OUI_SDL_Image.h"
#include "sdl2/OUI_SDL_Font.h"
#include "util/OUI_StringUtil.h"
#include <SDL_image.h>

#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

oui::SDLGraphics::~SDLGraphics() {
    if (buffer != NULL) {
        SDL_DestroyTexture(buffer);
    }
    buffer = NULL;
}

void oui::SDLGraphics::prepare() {
    SDL_SetRenderTarget(renderer, buffer);
    SDL_SetRenderDrawColor(renderer, currentColor.getR(), currentColor.getG(), currentColor.getB(), currentColor.getA());
}

oui::SDLGraphics::SDLGraphics(int width, int height, SDL_Renderer* renderer) : Graphics(width, height) {
    this->renderer = renderer;
    this->buffer = NULL;
    this->setSize(width, height);
}

oui::Image* oui::SDLGraphics::loadImage(const std::u16string& path, Window* window) {
    SDL_Surface* loadedSurface = IMG_Load(std::string("data/img/" + convertUTF16toUTF8(path)).c_str());
    if(loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", convertUTF16toUTF8(path).c_str(), IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(texture == NULL) {
        printf("Unable to load TEXTURE! %s! SDL_image Error: %s\n", convertUTF16toUTF8(path).c_str(), IMG_GetError());
        return NULL;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDLImage* img = new SDLImage(texture);
    SDL_FreeSurface(loadedSurface);
    Image::cacheImage(window, path, img);//TODO this doest get used
    return img;

}
oui::Font* oui::SDLGraphics::loadFont(const std::u16string& name, int size, Window* window) {
    TTF_Font* mFont = TTF_OpenFont(std::string("data/fonts/" + convertUTF16toUTF8(name) + ".ttf").c_str(), size);
    if (mFont == NULL) {
        std::cout << "Error loading font: name=" << convertUTF16toUTF8(name).c_str() << " size=" << size  << " " << TTF_GetError() << std::endl;
        Font* font = NULL;
        if (name != u"notoserif") {
            font =  Font::getFont(u"notoserif", size, window);
        }

        if (font == NULL) {
            throw "Could not load font";
        }
        return font;
    }
    int w, h;
    TTF_SizeText(mFont, "YO", &w, &h);
    if (h == 0 || w == 0) {
        std::cout << "The font was not properly loaded: name=" << convertUTF16toUTF8(name).c_str() << " size=" << size << std::endl;

    }
    Font* font = new SDLFont(name, size, mFont);
    return font;
}

void oui::SDLGraphics::setSize(int width, int height) {
    Graphics::setSize(width, height);
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

void oui::SDLGraphics::clear() {
    prepare();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
}

void oui::SDLGraphics::drawRect(int x, int y, int width, int height) {
    if (currentColor.getA() == 0) {
        return;
    }
    prepare();
    SDL_Rect rect = { x, y, width ,height };
    SDL_RenderDrawRect(renderer, &rect);
}

void oui::SDLGraphics::fillRect(int x, int y, int width, int height) {
    if (currentColor.getA() == 0) {
        return;
    }
    prepare();
    SDL_Rect rect = { x, y, width ,height };
    SDL_RenderFillRect(renderer, &rect);
}

void oui::SDLGraphics::drawLine(int x1, int y1, int x2, int y2) {
    if (currentColor.getA() == 0) {
        return;
    }
    prepare();
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void oui::SDLGraphics::fillVerticalGradient(int x, int y, int w, int h, oui::Color color1, oui::Color color2) {
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

void oui::SDLGraphics::drawImage(Image* image, int x, int y, int width, int height) {
    prepare();
    SDLImage* sdlImg = (SDLImage*) image;
    SDL_Rect dst = {x, y, width, height};
    SDL_RenderCopy(renderer, sdlImg->getBaseImage(), NULL, &dst);
}

void oui::SDLGraphics::drawImage(Image* image, int x, int y) {
    prepare();
    SDLImage* sdlImg = (SDLImage*) image;
    SDL_Rect dst = {x, y, image->getWidth(), image->getHeight()};
    SDL_RenderCopy(renderer, sdlImg->getBaseImage(), NULL, &dst);
}

void oui::SDLGraphics::drawText(const std::u16string& text, int x, int y) {
    auto lines = std::vector<std::u16string>();

    std::u16string currLine = u"";
    bool lastCharWasBackslash = false;
    for (int i = 0; i < text.length(); i++) {
        char c = (char) text.at(i);

        if (lastCharWasBackslash) {
            if (c == 'n') {
                lines.push_back(currLine);
                currLine = u"";
            } else if (c == '\\') {
                currLine += c;
            }

            lastCharWasBackslash = false;
            continue;
        }

        if (c == '\\') {
            lastCharWasBackslash = true;
            continue;
        }

        currLine += c;
    }

    if (currLine.length() > 0) {
        lines.push_back(currLine);
    }

    int lineHeight = this->font->getStringHeight(u"A");
    for(int i = 0; i < lines.size(); i++) {
        this->drawTextLine(lines[i], x, y + (lineHeight * i));
    }
 }

void oui::SDLGraphics::drawTextLine(const std::u16string& text, int x, int y) {
    if (text.length() == 0) {
        return;
    }
    if (font == NULL) {
        printf("Can't draw text");
        return;
    }

    SDLFont* sdlFont = (SDLFont*) font;

    SDL_Color foregroundColor = {(Uint8) currentColor.getR(), (Uint8) currentColor.getG(), (Uint8) currentColor.getB()};
    SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(sdlFont->getBaseFont(), reinterpret_cast<const Uint16*>(text.c_str()), foregroundColor);
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

void oui::SDLGraphics::setAlpha(int alpha) {
    Graphics::setAlpha(alpha);
    if (buffer != NULL) {
        SDL_SetTextureAlphaMod(buffer, alpha);
    }
}

void oui::SDLGraphics::renderToGraphics(int x, int y, Graphics* target) {
    ((SDLGraphics*) target)->prepare();
    SDL_Rect dest = {x, y, width, height};
    //std::cout << "graphics: width=" << width << " height=" << height << std::endl;
    SDL_RenderCopy(renderer, buffer, NULL, &dest);
}

void oui::SDLGraphics::renderToWindow(int x, int y) {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_Rect dest = {x, y, width, height};
    //std::cout << "graphics: width=" << width << " height=" << height << std::endl;
    SDL_RenderCopy(renderer, buffer, NULL, &dest);
}