#include "components/textfield/OUI_TextField.h"
#include <iostream>
#include <sstream>
#include "OUI_KeyCodes.h"
#include "window/OUI_window.h"
#include "OUI.h"
#include "util/OUI_StringUtil.h"
#include "event/OUI_KeyboardEvent.h"
#include "event/OUI_MenuEvent.h"
#include "attribute/OUI_AttributeNames.h"

using namespace oui::AttributeNames;

oui::TextField::~TextField() {
}

oui::TextField::TextField(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher, TextFieldAttributeManager* attributeManager) : 
    selectStart{0}, typing{false}, highlighting{false},
    caratVisible{true}, resetInput{false}, lastInput{0},
    caratIndex{0}, lastCaratSwitch{0}, drawX{0},
    Component("textfield", name, classes, true, eventDispatcher, attributeManager) {
    selectStart = 0;
    typing = false;

    // TODO use default styling like other comps
    setAttribute(TEXT, u"");
    parseAttribute(FONT, u"notoserif 14");
    setAttribute(BACKGROUND_COLOR_1, Color(240, 240, 240, 255));
    setAttribute(BACKGROUND_COLOR_2, Color(230, 230, 230, 255));
    setAttribute(BORDER_STYLE, u"solid");
    setAttribute(BORDER_COLOR, Color(0, 0, 0, 64));
    setAttribute(TEXT_COLOR, Color(0, 0, 0, 255));
    setAttribute(CURSOR, u"text");
    setAttribute(CARAT_WIDTH, 1);
    setAttribute(CARAT_COLOR, Color(0, 0, 0, 255));
    setAttribute(CARAT_H_OFFSET, 2);
    setAttribute(HIGHLIGHT_COLOR, Color(200, 200, 255, 255));
    parseAttribute(SIZE, u"0 0 150 25");
    
    addEventListener("mousedown", std::bind(&TextField::onMouseDown, this, std::placeholders::_1));
    addEventListener("keytyped", std::bind(&TextField::onKeyTyped, this, std::placeholders::_1));
}

void oui::TextField::addedToContainer(Container* parent) {
    Component::addedToContainer(parent);
    
    if (window != NULL) {
        window->addEventListener("mouseup", std::bind(&TextField::onMouseUp, this, std::placeholders::_1));
        window->addEventListener("mousemove", std::bind(&TextField::onMouseMove, this, std::placeholders::_1));
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


std::vector<std::u16string> oui::TextField::getRightClickOptions() {
    std::vector<std::u16string> options = attributeManager->getRightClickOptions();
    options.insert(options.begin(), u"Paste");
    options.insert(options.begin(), u"Copy");
    options.insert(options.begin(), u"Cut");
    return options;
}

void oui::TextField::redraw() {
    Component::redraw();
    TextFieldAttributeManager* attributeManager = getAttributeManager();
    int borderWidth = attributeManager->getBorderWidth();
    Font* font = attributeManager->getFont();
    std::u16string text = attributeManager->getText();
    int caratHeightOffset = attributeManager->getCaratHeightOffset();

    if (isSelected() && selectStart != caratIndex) {
        Color highlightColor = attributeManager->getHighlightColor();

        //Draw carat
        graphics->setColor(highlightColor);
        int startX = font->getStringWidth(text.substr(0, selectStart)) - drawX + borderWidth + 1;
        int endX = font->getStringWidth(text.substr(0, caratIndex)) - drawX + borderWidth + 1;
        graphics->fillRect(startX, caratHeightOffset, endX - startX, getHeight() - caratHeightOffset * 2);
    }

    if (text != u"") {
        Color textColor = attributeManager->getTextColor();
        graphics->setColor(textColor);
        graphics->setFont(font);
        graphics->drawTextLine(text, -drawX + borderWidth + 1, getHeight() / 2 - font->getStringHeight(text) / 2);
    }

    if (isSelected()) {
        if (caratVisible) {
            Color caratColor = attributeManager->getCaratColor();
            int caratWidth = attributeManager->getCaratWidth();

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

void oui::TextField::onMenuOption(ComponentEvent* compEvent) {
    TextFieldAttributeManager* attributeManager = getAttributeManager();
    std::u16string text = attributeManager->getText();

    MenuEvent* event = static_cast<MenuEvent*>(compEvent);
    std::u16string option = event->option;

    if (option == u"Cut") {
        if (caratIndex != selectStart) {
            EditEvent* e = new EditEvent(getUndoEvent(), [this, text] {
                bool reverse = selectStart > caratIndex;
                int start = reverse ? caratIndex : selectStart;
                int end = reverse ? selectStart : caratIndex;
                static_cast<Window*>(window)->setClipboardText(text.substr(start, end));
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
        if (static_cast<Window*>(window)->getClipboardText() != u"") {
            EditEvent* e = new EditEvent(getUndoEvent(), [this] {
                insertString(static_cast<Window*>(window)->getClipboardText());
            });
            e->performRedo();
            static_cast<Window*>(window)->addEditEvent(e);
        }
    }
}

void oui::TextField::onMouseDown(ComponentEvent* compEvent) {
    MouseEvent* event = static_cast<MouseEvent*>(compEvent);
    caratVisible = true;
    selectStart = 0;
    setCaratIndex(getIndexAt(event->localX));
    selectStart = caratIndex;
    highlighting = true;
}
void oui::TextField::onMouseUp(ComponentEvent* e) {
    highlighting = false;
}

void oui::TextField::onMouseMove(ComponentEvent* compEvent) {
    MouseEvent* event = static_cast<MouseEvent*>(compEvent);
    if (this->highlighting) {
        int localX = event->windowX - getScreenX();
        setCaratIndex(getIndexAt(localX));
    }
}
void oui::TextField::onKeyTyped(ComponentEvent* compEvent) {
    TextFieldAttributeManager* attributeManager = getAttributeManager();
    std::u16string text = attributeManager->getText();
    KeyboardEvent* event = static_cast<KeyboardEvent*>(compEvent);
    int code = event->key;
    char character = event->character;
    if (code == KEY_BACKSPACE) {
        if (caratIndex != 0 || caratIndex != selectStart) {
            EditEvent* e = new EditEvent(getUndoEvent(), [this, character] {
                deleteChar(true);
            }, false, true, this);
            e->performRedo();
            static_cast<Window*>(window)->addEditEvent(e, currentTimeMillis() - lastInput < 512 && !typing && !resetInput);
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
            static_cast<Window*>(window)->addEditEvent(e, currentTimeMillis() - lastInput < 512);
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
    } else if (event->ctrlKey) {
        if (code == KEY_C) {
            bool reverse = selectStart > caratIndex;
            int start = reverse ? caratIndex : selectStart;
            int end = reverse ? selectStart : caratIndex;
            if (start != end) {
                static_cast<Window*>(window)->setClipboardText(text.substr(start, end));
            }
            resetInput = true;
        }
        if (code == KEY_X) {
            if (caratIndex != selectStart) {
                EditEvent* e = new EditEvent(getUndoEvent(), [this, character, text] {
                    bool reverse = selectStart > caratIndex;
                    int start = reverse ? caratIndex : selectStart;
                    int end = reverse ? selectStart : caratIndex;
                    static_cast<Window*>(window)->setClipboardText(text.substr(start, end));
                    deleteChar(true);
                });
                e->performRedo();
                static_cast<Window*>(window)->addEditEvent(e);
            }
            resetInput = true;
        }
        if (code == KEY_V) {
            if (static_cast<Window*>(window)->getClipboardText() != u""){
                EditEvent* e = new EditEvent(getUndoEvent(), [this, character] {
                    insertString(static_cast<Window*>(window)->getClipboardText());
                });
                e->performRedo();
                static_cast<Window*>(window)->addEditEvent(e);
            }
            resetInput = true;
        }
    } else if (character == ' ' && code != KEY_SPACE) {
        resetInput = true;
    } else {
        EditEvent* e = new EditEvent(getUndoEvent(), [this, character] {
            insertChar(character);
        }, false, true, this);
        e->performRedo();
        static_cast<Window*>(window)->addEditEvent(e, currentTimeMillis() - lastInput < 512 && code != KEY_ENTER && typing && !resetInput);
        lastInput = currentTimeMillis();
        typing = true;
        resetInput = false;
    }
}

void oui::TextField::setSelected(bool selected) {
    Component::setSelected(selected);
    if (!selected) {
        flagGraphicsUpdate();
    }
}

void oui::TextField::setText(const std::u16string& text) {
    setAttribute(TEXT, text);
}

void oui::TextField::insertString(const std::u16string& string) {
    TextFieldAttributeManager* attributeManager = getAttributeManager();
    std::u16string text = attributeManager->getText();
    if (selectStart != caratIndex) {
        deleteChar(true);
    }
    for (unsigned int i = 0; i < string.length(); i++) {//This is a dumb way to do it
        text.insert(caratIndex, 1, string.at(i));
        moveCarat(true);
    }
    flagGraphicsUpdate();
    setAttribute(TEXT, text);
}
void oui::TextField::insertChar(char c) {
    TextFieldAttributeManager* attributeManager = getAttributeManager();
    std::u16string text = attributeManager->getText();
    if (selectStart != caratIndex) {
        deleteChar(true);
    }
    text.insert(caratIndex, 1, c);
    moveCarat(true);
    flagGraphicsUpdate();
    setAttribute(TEXT, text);
}
void oui::TextField::deleteChar(bool backspace) {
    TextFieldAttributeManager* attributeManager = getAttributeManager();
    std::u16string text = attributeManager->getText();
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
            text.erase(caratIndex + (size_t) (backspace ? -1 : 0));
            if (backspace) {
                moveCarat(false);
            } else {
                updateTextPosition();
            }
        }
    }
    setAttribute(TEXT, text);
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
    TextFieldAttributeManager* attributeManager = getAttributeManager();
    std::u16string text = attributeManager->getText();
    Font* font = attributeManager->getFont();
    int borderWidth = attributeManager->getBorderWidth();
    int caratWidth = attributeManager->getCaratWidth();

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
    TextFieldAttributeManager* attributeManager = getAttributeManager();
    std::u16string text = attributeManager->getText();
    Font* font = attributeManager->getFont();
    int borderWidth = attributeManager->getBorderWidth();

    int lastWidth = font->getStringWidth(std::u16string(text));
    for (int i = (int) text.length() - 1; i >= 0; i--) {
        std::u16string testStr = text.substr(0, i);
        int strWidth = font->getStringWidth(testStr);
        int halfPoint = strWidth + (lastWidth - strWidth) / 2 + borderWidth + 1;
        if (x + drawX > halfPoint) {
            return (int) i + 1;
        }
        lastWidth = strWidth;
    }
    return 0;
}
std::function<void()> oui::TextField::getUndoEvent() {
    TextFieldAttributeManager* attributeManager = getAttributeManager();
    std::u16string text = attributeManager->getText();
    int selectStart_ = selectStart;
    int caratIndex_ = caratIndex;
    std::u16string text_ = text;
    return [this, selectStart_, caratIndex_, text_] {
        selectStart = selectStart_;
        caratIndex = caratIndex_;
        setText(text_);
        updateTextPosition();
        if (window != NULL) {
            static_cast<Window*>(window)->setSelectedComponent(this);
        }
    };
}

oui::TextFieldAttributeManager* oui::TextField::getAttributeManager() {
    return static_cast<TextFieldAttributeManager*>(attributeManager);
}