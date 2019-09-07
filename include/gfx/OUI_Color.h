
#ifndef OUI_COLOR_H
#define OUI_COLOR_H

#include "OUI_Export.h"
#include <stdint.h>

namespace oui {

    class OUI_API Color {
        public: static Color fromString(std::u16string string);

        public: static Color NONE;
        public: static Color WHITE;
        public: static Color BLACK;

        private:uint32_t rgba;

        public: Color();
        public: Color(Color& color);
        public: explicit Color(uint32_t rgba);
        public: Color(int r, int g, int b, int a = 255);

        public:int getARGB();
        public:int getA();
        public:int getR();
        public:int getG();
        public:int getB();

        public: Color setA(int a);

        public: bool equals(Color c);
        public: std::string toString();
    };

}

#endif