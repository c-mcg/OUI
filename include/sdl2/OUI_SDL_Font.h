#ifndef OUI_SDL_FONT_H
#define OUI_SDL_FONT_H

#include "oui/gfx/OUI_Font.h"

#include <unordered_map>

typedef struct _TTF_Font TTF_Font;

namespace oui {

    class Window;

    class SDLFont : public Font {

        private: static Font* loadFont(const std::u16string& name, int size);

        private: TTF_Font* baseFont;

        public: SDLFont(const std::u16string& name, int size, TTF_Font* baseFont);

        public: TTF_Font* getBaseFont();

        public: int getStringHeight(const std::u16string& string) override;
        public: int getStringWidth(const std::u16string& string) override;


    };

}

#endif
