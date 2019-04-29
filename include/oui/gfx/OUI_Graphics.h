#ifndef OUI_GRAPHICS_H
#define OUI_GRAPHICS_H

#include "OUI_Export.h"
#include "oui/gfx/OUI_Color.h"
#include "oui/gfx/OUI_Image.h"
#include "oui/gfx/OUI_Font.h"

namespace oui {

    class Window;

    class OUI_API Graphics {

        protected: int width;
        protected: int height;
        protected: int alpha;

        protected: Color currentColor;
        protected: Font* font;

        public: ~Graphics();
        protected: Graphics(int width, int height);
        
        public: int getWidth();
        public: int getHeight();

        public: void setColor(Color color);
        public: Color getColor();

        public: void setFont(Font* font);
        public: Font* getFont();

        
        public: int getAlpha();

        //Abstract
        public: virtual Image* loadImage(const std::u16string& path, Window* window);
        public: virtual Font* loadFont(const std::u16string& path, int size, Window* window);

        public: virtual void setAlpha(int alpha);

        public: virtual void renderToGraphics(int x, int y, Graphics* target);
        public: virtual void renderToWindow(int x, int y);
        public: virtual void setSize(int width, int height);

        public: virtual void clear();
        public: virtual void drawRect(int x, int y, int width, int height);
        public: virtual void fillRect(int x, int y, int width, int height);
        public: virtual void drawLine(int x1, int y1, int x2, int y2);
        public: virtual void fillVerticalGradient(int x, int y, int w, int h, Color color1, Color color2);
        public: virtual void drawImage(Image* image, int x, int y);
        public: virtual void drawImage(Image* image, int x, int y, int width, int height);
        public: virtual void drawText(const std::u16string& text, int x, int y);
        public: virtual void drawTextLine(const std::u16string& text, int x, int y);
    };

}

#endif