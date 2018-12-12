#ifndef OUI_SDL_GRAPHICS_H
#define OUI_SDL_GRAPHICS_H

#include "oui/gfx/OUI_Graphics.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Surface;

namespace oui {

    class Window;

	class SDLGraphics : public Graphics {

		protected: SDL_Renderer* renderer;
		protected: SDL_Texture* buffer;

		public: ~SDLGraphics();
		public: SDLGraphics(int width, int height, SDL_Renderer* renderer);
		
        public: void prepare();

        public: virtual Image* loadImage(std::u16string path, Window* window);
        public: virtual Font* loadFont(std::u16string path, int size, Window* window);

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
		public: virtual void drawText(std::u16string text, int x, int y);


	};

}

#endif