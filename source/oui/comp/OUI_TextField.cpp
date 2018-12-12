#include "oui/comp/OUI_TextField.h"
#include <iostream>
#include <sstream>
#include "OUI_KeyCodes.h"
#include "OUI_Constants.h"
#include "oui/OUI_Window.h"
#include "OUI.h"
#include "util/OUI_StringUtil.h"

oui::TextField::~TextField() {
	font = NULL;
}

oui::TextField::TextField(std::string name, std::string classes) : Component("button", name, classes, true){
	selectStart = 0;
	typing = true;
	setAttribute("text", u"");
	parseAttribute("font", u"notoserif 14");
	parseAttribute("bg-color1", u"240 240 240 255");
	parseAttribute("bg-color2", u"230 230 230 255");
	setAttribute("border-style", u"solid");
	parseAttribute("border-color", u"0 0 0 64");
	parseAttribute("text-color", u"0 0 0 255");
	setAttribute("cursor", u"text");
	setAttribute("carat-width", 1);
	parseAttribute("carat-color", u"0 0 0 255");
	setAttribute("carat-h-offset", 2);
	parseAttribute("highlight-color", u"200 200 255 255");
	parseAttribute("size", u"0 0 150 25");
}

void oui::TextField::addedToContainer(Container* parent) {
	Component::addedToContainer(parent);
	
	if (window != NULL) {
        window->addEventListener(Event::MOUSE_UP, [this](MouseEvent e, Component* c) {
			this->highlighting = false;
		});
		window->addEventListener(Event::MOUSE_MOVE, [this](MouseEvent e, Component* c) {
            if (this->highlighting) {
			    setCaratIndex(getIndexAt(e.getGlobalX() - this->getScreenX()));
            }
		});
	}
}

int oui::TextField::process() {
	Component::process();

	if (isSelected()) {
		if (currentTimeMillis() - lastCaratSwitch >= 600) {
			caratVisible = !caratVisible;
			lastCaratSwitch = currentTimeMillis();
			flagGraphicsUpdate();
		}
	}
	return 0;
}

void oui::TextField::setProfile(std::u16string profileName) {
	Component::setProfile(profileName);

	AttributeProfile* profile = style->getProfile(profileName);
	if (profile != NULL) {

		//Text
		text = profile->getString("text");

		//Font
		font = Font::getFont(profile->getString("font-face"), profile->getInt("font-size"), window);

		//Text-color
		textColor =Color(profile->getInt("text-color-r"), profile->getInt("text-color-g"), profile->getInt("text-color-b"), profile->getInt("text-color-a"));

		//Carat-width
		caratWidth = profile->getInt("carat-width");
		//Carat-color
		caratColor = Color(profile->getInt("carat-color-r"), profile->getInt("carat-color-g"), profile->getInt("carat-color-b"), profile->getInt("carat-color-a"));
		//Carat-h-offset
		caratHeightOffset = profile->getInt("carat-h-offset");

		//highlight-color
		highlightColor = Color(profile->getInt("highlight-color-r"), profile->getInt("highlight-color-g"), profile->getInt("highlight-color-b"), profile->getInt("highlight-color-a"));

	}
}

std::vector<std::u16string> oui::TextField::getRightClickOptions() {
	std::vector<std::u16string> options = Component::getRightClickOptions();
	options.insert(options.begin(), u"Paste");
	options.insert(options.begin(), u"Copy");
	options.insert(options.begin(), u"Cut");
	return options;
}

void oui::TextField::redraw() {
	Component::redraw();
	int borderWidth = getCurrentProfile()->getInt("border-width");
	if (isSelected() && selectStart != caratIndex) {
			//Draw carat
		graphics->setColor(highlightColor);
		int startX = font->getStringWidth(text.substr(0, selectStart)) - drawX + borderWidth + 1;
		int endX = font->getStringWidth(text.substr(0, caratIndex)) - drawX + borderWidth + 1;
		graphics->fillRect(startX, caratHeightOffset, endX - startX, getHeight() - caratHeightOffset * 2);
	}
	if (text != u"") {
		graphics->setColor(textColor);
		graphics->setFont(font);
		graphics->drawText(text, -drawX + borderWidth + 1, getHeight() / 2 - font->getStringHeight(text) / 2);
	}
	if (isSelected()) {
		if (caratVisible) {
			//Draw carat
			graphics->setColor(caratColor);
			int caratX = font->getStringWidth(text.substr(0, caratIndex)) - drawX + borderWidth + 1;
			for (int i = 0; i < caratWidth; i++) {
				graphics->drawLine(caratX + i, caratHeightOffset, caratX + i, getHeight() - caratHeightOffset * 2);
			}
		}
	}
	drawBorder();
}

void oui::TextField::handleEvent(Event& e) {

	if (e.isMenuEvent()) {

		MenuEvent& menuE = (MenuEvent&) e;
		std::u16string option = menuE.getOption();
		if (option == u"Cut") {
			if (caratIndex != selectStart) {
				EditEvent* e = new EditEvent(getUndoEvent(), [this] {
					bool reverse = selectStart > caratIndex;
					int start = reverse ? caratIndex : selectStart;
					int end = reverse ? selectStart : caratIndex;
					((Window*) window)->setClipboardText(text.substr(start, end));
					deleteChar(true);
				});
				e->performRedo();
				window->addEditEvent(e);
			}
		}

		if (option == u"Copy") {
			bool reverse = selectStart > caratIndex;
			int start = reverse ? caratIndex : selectStart;
			int end = reverse ? selectStart : caratIndex;
			if (start != end) {
				window->setClipboardText(text.substr(start, end));
			}
		}

		if (option == u"Paste") {
			if (((Window*) window)->getClipboardText() != u"") {
				EditEvent* e = new EditEvent(getUndoEvent(), [this] {
					insertString(((Window*) window)->getClipboardText());
				});
				e->performRedo();
				((Window*) window)->addEditEvent(e);
			}
		}

	} else if (e.isMouseEvent()) {

		MouseEvent& mouseEvt = (MouseEvent&) e;
		int evtX = mouseEvt.getX();
		int evtY = mouseEvt.getY();

		if (e.type == e.MOUSE_DOWN) {
			caratVisible = true;
			selectStart = 0;
			setCaratIndex(getIndexAt(evtX));
			selectStart = caratIndex;
			highlighting = true;
		}

		if (e.type == e.MOUSE_UP) {
			highlighting = false;
		}

	} else if (e.isKeyEvent()) {
		KeyEvent& keyEvt = (KeyEvent&) e;

		if (e.type == e.KEY_TYPED) {
			int code = keyEvt.getKeyCode();
			char character = keyEvt.getKeyChar();;
			if (code == KEY_BACKSPACE) {
				if (caratIndex != 0 || caratIndex != selectStart) {
					EditEvent* e = new EditEvent(getUndoEvent(), [this, character] {
						deleteChar(true);
					}, false, true, this);
					e->performRedo();
					((Window*) window)->addEditEvent(e, currentTimeMillis() - lastInput < 512 && !typing && !resetInput);
					typing = false;
					resetInput = false;
					lastInput = currentTimeMillis();
				}
			} else if (code == KEY_DELETE) {
				if (caratIndex != text.length() || caratIndex != selectStart) {
					EditEvent* e = new EditEvent(getUndoEvent(), [this, character] {
						deleteChar(false);
					}, false, !typing && !resetInput, this);
					e->performRedo();
					((Window*) window)->addEditEvent(e, currentTimeMillis() - lastInput < 512);
					typing = false;
					resetInput = false;
					lastInput = currentTimeMillis();
				}
			} else if (code == KEY_LEFT) {
				if (caratIndex != 0 || caratIndex != selectStart) {
					moveCarat(false);
				}
				resetInput = true;
			} else if (code == KEY_RIGHT) {
				if (caratIndex != text.length() || caratIndex != selectStart) {
					moveCarat(true);
				}
				resetInput = true;
			} else if (window != NULL && ((Window*) window)->isCtrlDown()) {
				if (code == KEY_C) {
					bool reverse = selectStart > caratIndex;
					int start = reverse ? caratIndex : selectStart;
					int end = reverse ? selectStart : caratIndex;
					if (start != end) {
						((Window*) window)->setClipboardText(text.substr(start, end));
					}
					resetInput = true;
				}
				if (code == KEY_X) {
					if (caratIndex != selectStart) {
						EditEvent* e = new EditEvent(getUndoEvent(), [this, character] {
							bool reverse = selectStart > caratIndex;
							int start = reverse ? caratIndex : selectStart;
							int end = reverse ? selectStart : caratIndex;
							((Window*) window)->setClipboardText(text.substr(start, end));
							deleteChar(true);
						});
						e->performRedo();
						((Window*) window)->addEditEvent(e);
					}
					resetInput = true;
				}
				if (code == KEY_V) {
					if (((Window*) window)->getClipboardText() != u"") {
						EditEvent* e = new EditEvent(getUndoEvent(), [this, character] {
							insertString(((Window*) window)->getClipboardText());
						});
						e->performRedo();
						((Window*) window)->addEditEvent(e);
					}
					resetInput = true;
				}
			} else if (character == ' ' && code != KEY_SPACE) {
				resetInput = true;
			} else {
				int selectStart_ = selectStart;
				int caratIndex_ = caratIndex;
				std::u16string text_ = text;
				EditEvent* e = new EditEvent(getUndoEvent(), [this, character] {
					insertChar(character);
				}, false, true, this);
				e->performRedo();
				((Window*) window)->addEditEvent(e, currentTimeMillis() - lastInput < 512 && code != KEY_ENTER && typing && !resetInput);
				lastInput = currentTimeMillis();
				typing = true;
				resetInput = false;
			}
		}
    }
	Component::handleEvent(e);
}
void oui::TextField::setSelected(bool selected) {
	Component::setSelected(selected);
	if (!selected) {
		flagGraphicsUpdate();
	}
}

void oui::TextField::setText(std::u16string text) {
	setAttribute("text", text);
}

void oui::TextField::insertString(std::u16string string) {
	if (selectStart != caratIndex) {
		deleteChar(true);
	}
	for (unsigned int i = 0; i < string.length(); i++) {//This is a dumb way to do it
		text.insert(caratIndex, 1, string.at(i));
		moveCarat(true);
	}
	flagGraphicsUpdate();
	setAttribute("text", text);
}
void oui::TextField::insertChar(char c) {
	if (selectStart != caratIndex) {
		deleteChar(true);
	}
	text.insert(caratIndex, 1, c);
	moveCarat(true);
	flagGraphicsUpdate();
	setAttribute("text", text);
}
void oui::TextField::deleteChar(bool backspace) {
	if (selectStart != caratIndex) {
		bool reverse = selectStart > caratIndex;
		if (text.size() > 0 && caratIndex >= 0 && caratIndex <= text.size() && selectStart >= 0 && selectStart <= text.size()) {

			text.erase(reverse ? caratIndex : selectStart, reverse ? selectStart : caratIndex);

			if (reverse) {
				selectStart = getCaratIndex();
				flagGraphicsUpdate();
			} else {
				setCaratIndex(selectStart);
			}
		}
		
	} else {
		if (text == u"" || (caratIndex == 0 && backspace) || (caratIndex == text.length() && !backspace)) {
			//return;
		} else {
			text.erase(caratIndex + (backspace ? -1 : 0));
			if (backspace) {
				moveCarat(false);
			} else {
				updateTextPosition();
			}
		}
	}
	setAttribute("text", text);
}
void oui::TextField::moveCarat(bool right) {
	if (caratIndex == selectStart) {
		caratIndex += right ? 1 : -1;
		caratVisible = true;
	}
	lastCaratSwitch = currentTimeMillis();
	selectStart = caratIndex;
	updateTextPosition();
}

int oui::TextField::getCaratIndex() {
	return caratIndex;
}
void oui::TextField::setCaratIndex(int caratIndex) {
	this->caratIndex = caratIndex;
	lastCaratSwitch = currentTimeMillis();
	updateTextPosition();
}

void oui::TextField::updateTextPosition() {
	int borderWidth = getCurrentProfile()->getInt("border-width");
	int caratX = font->getStringWidth(text.substr(0, caratIndex));
	if (caratX < drawX) {
		drawX = caratX;
	} else if (caratX > drawX + getWidth() - (borderWidth + 1) * 2 - caratWidth) {
		drawX = caratX - getWidth() + (borderWidth + 1) * 2 + caratWidth ;
	}

	int textWidth = font->getStringWidth(text);
	if (drawX > textWidth - getWidth() + ( + 1) * 2 + caratWidth) {
		drawX = textWidth - getWidth() + (borderWidth + 1) * 2 + caratWidth;
	}
	if (drawX < 0 || textWidth < getWidth() - (borderWidth + 1) * 2) {
		drawX = 0;
	}
	flagGraphicsUpdate();
}

int oui::TextField::getIndexAt(int x) {
	int borderWidth = getCurrentProfile()->getInt("border-width");
	std::u16string lastStr = std::u16string(text);
	int lastWidth = font->getStringWidth(lastStr);
	int strWidth = 0;
	for (int i = text.length() - 1; i >= 0; i--) {
		std::u16string testStr = text.substr(0, i);
		strWidth = font->getStringWidth(testStr);
		int halfPoint = strWidth + (lastWidth - strWidth) / 2 + borderWidth + 1;
		if (x + drawX > halfPoint) {
			return i + 1;
		}
		lastStr = testStr;
		lastWidth = strWidth;
	}
	return 0;
}
std::function<void()> oui::TextField::getUndoEvent() {
	int selectStart_ = selectStart;
	int caratIndex_ = caratIndex;
	std::u16string text_ = text;
	return [this, selectStart_, caratIndex_, text_] {
		selectStart = selectStart_;
		caratIndex = caratIndex_;
		setText(text_);
		updateTextPosition();
		if (window != NULL) {
			((Window*) window)->setSelectedComponent(this);
		}
	};
}