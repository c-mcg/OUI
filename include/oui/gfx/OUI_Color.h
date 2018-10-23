
#ifndef OUI_COLOR_H
#define OUI_COLOR_H

#include "OUI_Export.h"
#include <stdint.h>

namespace oui {

	class OUI_API Color {
		public: static Color BLACK;

		private:uint32_t rgba;

		public:Color();
		public:Color(uint32_t rgba);
		public:Color(int r, int g, int b, int a = 255);

		public:int getARGB();
		public:int getA();
		public:int getR();
		public:int getG();
		public:int getB();

		public: bool equals(Color c);
	};

}

#endif