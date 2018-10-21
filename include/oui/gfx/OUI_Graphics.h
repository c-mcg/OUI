#ifndef OUI_GRAPHICS_H
#define OUI_GRAPHICS_H

#include "OUI_Export.h"
#include "oui/gfx/OUI_Color.h"
#include "oui/gfx/OUI_Image.h"
#include "oui/gfx/OUI_Font.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Surface;

namespace oui {

	class OUI_API Graphics {

		private: int width;
		private: int height;
		private: int alpha;

		private: SDL_Renderer* renderer;
		private: SDL_Texture* buffer;
		private: Color currentColor;
		private: Font* font;

		public: ~Graphics();
		public: Graphics(int width, int height);

		public: void setRenderer(SDL_Renderer* renderer);
		public: SDL_Renderer* getRenderer();
		public: void setSize(int width, int height);
		public: int getWidth();
		public: int getHeight();

		public: void setColor(oui::Color color);
		public: Color getColor();

		public: void setFont(Font* font);
		public: Font* getFont();

		public: void clear();
		public: void drawRect(int x, int y, int width, int height);
		public: void fillRect(int x, int y, int width, int height);
		public: void drawLine(int x1, int y1, int x2, int y2);
		public: void fillVerticalGradient(int x, int y, int w, int h, oui::Color color1, oui::Color color2);
		public: void drawImage(Image* image, int x, int y);
		public: void drawImage(Image* image, int x, int y, int width, int height);
		public: void drawText(std::u16string text, int x, int y);

		public: void setAlpha(int alpha);
		public: int getAlpha();

		public: void render(int x, int y);

		public: void prepare();

	};

}

#endif