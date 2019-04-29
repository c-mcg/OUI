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

        public: Image* loadImage(const std::u16string& path, Window* window) override;
        public: Font* loadFont(const std::u16string& path, int size, Window* window) override;

		public: void setAlpha(int alpha) override;

		public: void renderToGraphics(int x, int y, Graphics* target) override;
		public: void renderToWindow(int x, int y) override;
		public: void setSize(int width, int height) override;

        public: void clear() override;
		public: void drawRect(int x, int y, int width, int height) override;
		public: void fillRect(int x, int y, int width, int height) override;
		public: void drawLine(int x1, int y1, int x2, int y2) override;
		public: void fillVerticalGradient(int x, int y, int w, int h, Color color1, Color color2) override;
		public: void drawImage(Image* image, int x, int y) override;
		public: void drawImage(Image* image, int x, int y, int width, int height) override;
		public: void drawText(const std::u16string& text, int x, int y) override;
		public: void drawTextLine(const std::u16string& text, int x, int y) override;
	};

}

#endif