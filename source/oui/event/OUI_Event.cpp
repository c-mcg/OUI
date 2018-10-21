
#include "oui/event/OUI_Event.h"

oui::Event::Event(char type) {
	this->type = type;
}

bool oui::Event::isMouseEvent() {
	return false;
}
bool oui::Event::isKeyEvent() {
	return false;
}
bool oui::Event::isScrollEvent() {
	return type == MOUSE_WHEEL;
}
bool oui::Event::isMenuEvent() {
	return type == MENU_CLICKED;
}
bool oui::Event::isWindowEvent() {
	return false;
}

void oui::Event::preventDefault() {
	defaultPrevented = true;
}

oui::MouseEvent::MouseEvent(char type, int x, int y) : Event(type) {
	this->x = x;
	this->y = y;
}

bool oui::MouseEvent::isMouseEvent() {
	return true;
}

int oui::MouseEvent::getX() {
	return x;
}

int oui::MouseEvent::getY() {
	return y;
}

oui::KeyEvent::KeyEvent(char type, int keyCode, char keyChar) : Event(type) {
	this->keyCode = keyCode;
	this->keyChar = keyChar;
}

bool oui::KeyEvent::isKeyEvent() {
	return true;
}

int oui::KeyEvent::getKeyCode() {
	return keyCode;
}

char oui::KeyEvent::getKeyChar() {
	return keyChar;
}

oui::ScrollEvent::ScrollEvent(int scroll) : Event(MOUSE_WHEEL) {
	this->scroll = scroll;
}

int oui::ScrollEvent::getScroll() {
	return scroll;
}

oui::MenuEvent::MenuEvent(int index, std::u16string option) : Event(MENU_CLICKED) {
	this->index = index;
	this->option = option;
}

int oui::MenuEvent::getIndex() {
	return index;
}

std::u16string oui::MenuEvent::getOption() {
	return option;
}

oui::WindowEvent::WindowEvent(char type) : Event(type) {

}

bool oui::WindowEvent::isWindowEvent() {
	return true;
}