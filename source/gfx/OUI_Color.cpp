
#include "gfx/OUI_Color.h"

oui::Color oui::Color::BLACK = Color(0, 0, 0);

oui::Color::Color() : Color(255, 255, 255) {

}

oui::Color::Color(uint32_t rgba) {
    this->rgba = rgba;
}

oui::Color::Color(int r, int g, int b, int a) {
    rgba = 0;
    rgba |= a;
    rgba |= b << 8;
    rgba |= g << 16;
    rgba |= r << 24;
}

int oui::Color::getARGB() {
    return rgba;
}
int oui::Color::getA() {
    return rgba & 0xFF;
}
int oui::Color::getR() {
    return rgba >> 24 & 0xFF;
}
int oui::Color::getG() {
    return rgba >> 16 & 0xFF;
}
int oui::Color::getB() {
    return rgba >> 8 & 0xFF;
}

bool oui::Color::equals(oui::Color c) {
    return rgba == c.rgba;
}