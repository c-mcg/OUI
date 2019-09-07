
#include "gfx/OUI_Color.h"
#include "util/OUI_StringUtil.h"
#include <iostream>

oui::Color oui::Color::NONE = Color(0, 0, 0, 0);
oui::Color oui::Color::WHITE = Color(255, 255, 255);
oui::Color oui::Color::BLACK = Color(0, 0, 0);

oui::Color oui::Color::fromString(std::u16string value) {
    if (stringStartsWith(value, u"#")) {
        value = value.substr(1);
        uint32_t colorVal = std::stoul(std::string("0x") + convertUTF16toUTF8(value), nullptr, 16); 
        oui::Color color = oui::Color(colorVal);
        uint32_t rgba = 0;
        if (value.length() <= 6) {
            rgba |= color.getB(); 
            rgba |= color.getG() << 8;
            rgba |= color.getR() << 16;
            rgba |= 255 << 24;
        } else {
            rgba |= color.getA(); 
            rgba |= color.getB() << 8;
            rgba |= color.getG() << 16;
            rgba |= color.getR() << 24;
        }
        std::cout << "Color: #" << convertUTF16toUTF8(value).c_str() << "=" << Color(rgba).toString().c_str() << std::endl;
        return Color(rgba);
    }

    return Color::WHITE;
}


oui::Color::Color(Color& color) {
    this->rgba = color.rgba;
}

oui::Color::Color() : Color(255, 255, 255) {

}

oui::Color::Color(uint32_t rgba) {
    this->rgba = rgba;
}
oui::Color::Color(int r, int g, int b, int a) {
    rgba = 0;
    rgba |= r;
    rgba |= g << 8;
    rgba |= b << 16;
    rgba |= a << 24;
}

int oui::Color::getARGB() {
    return rgba;
}
int oui::Color::getR() {
    return rgba & 0xFF;
}
int oui::Color::getG() {
    return rgba >> 8 & 0xFF;
}
int oui::Color::getB() {
    return rgba >> 16 & 0xFF;
}
int oui::Color::getA() {
    return rgba >> 24 & 0xFF;
}


oui::Color oui::Color::setA(int a) {
    return Color(rgba | a << 24);
}

bool oui::Color::equals(oui::Color c) {
    return rgba == c.rgba;
}

std::string oui::Color::toString() {
    return "(" + std::to_string(getR()) + ", " +
        std::to_string(getG()) + ", " + 
        std::to_string(getB()) + ", " + 
        std::to_string(getA()) + ")";
}