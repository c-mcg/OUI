#include "components/menu/OUI_Menu.h"

#include <algorithm>
#include <set>

#include "event/OUI_MenuEvent.h"
#include "util/OUI_StringUtil.h"

#include "attribute/OUI_AttributeNames.h"

using namespace oui::AttributeNames;

oui::Menu::~Menu() {

}

oui::Menu::Menu(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher, MenuAttributeManager* attributeManager) : 
    target{NULL},
    Container("menu", name, classes, false, eventDispatcher, attributeManager) {
    //setSize(0, 0, 150, 30);

    // TODO use default style like other components
    setAttribute(BORDER_STYLE, u"solid");
    setAttribute(BORDER_COLOR, Color(0, 0, 0, 128));
    setAttribute(BACKGROUND_COLOR_1, Color::WHITE);
    setAttribute(BACKGROUND_COLOR_2, Color::WHITE);

    setAttribute(HOVER_COLOR, Color(200, 200, 200, 255));
    setAttribute(PADDING, 4);
    setAttribute(MIN_WIDTH, 100);
    setAttribute(OPTION_HEIGHT, 20);
    parseAttribute(FONT, u"notoserif 14");

    setAttribute(OPTIONS, std::vector<Attribute>());
}



bool oui::Menu::_addChild(Button* child) {
    return Container::addChild(child);
}

bool oui::Menu::addChild(Component* child) {
    std::cout << "You can't add children to menus, use addOption instead" << std::endl;
    return false;
}

oui::Button* oui::Menu::addOption(const std::u16string& option, int index) {
    MenuAttributeManager* attributeManager = getAttributeManager();
    int padding = attributeManager->getPadding();
    int optionHeight = attributeManager->getOptionHeight();
    std::u16string fontName = attributeManager->getFontName();
    int fontSize = attributeManager->getFontSize();
    Color hoverColor = attributeManager->getHoverColor();
    int minWidth = attributeManager->getMinWidth();
    int numOptions = attributeManager->getNumOptions();

    //Create button for option
    Button* b = new Button("option_" + std::to_string(index), "menuOption");
    b->setAttribute(PERMANENT, true);

    b->setAttribute(TEXT, option);

    int borderWidth = attributeManager->getBorderWidth();

    //Set size
    b->setAttribute(WIDTH_PERCENT, 100);
    b->setAttribute(HEIGHT_PERCENT, 0);
    b->setAttribute(WIDTH_OFFSET, -padding * 2 - borderWidth * 2);
    b->setAttribute(HEIGHT_OFFSET, optionHeight);

    //Set location
    b->setAttribute(X_PERCENT, 0);
    b->setAttribute(Y_PERCENT, 0);
    b->setAttribute(X_OFFSET, padding + borderWidth);
    b->setAttribute(Y_OFFSET, optionHeight * index + padding + borderWidth);

    //Font
    b->setAttribute(FONT_FACE, fontName);
    b->setAttribute(FONT_SIZE, fontSize);

    b->setAttribute(BORDER_STYLE, u"none");
    b->setAttribute(CENTER_TEXT, false);

    //Apply hover colour
    Color c = hoverColor;
    b->setAttribute(BACKGROUND_COLOR_1, c, u"hover");
    b->setAttribute(BACKGROUND_COLOR_2, c, u"hover");

    //Add button click event handler
    b->addEventListener("click", [this, b](ComponentEvent* e) {
        auto target = this->target == NULL ? this : this->target;

        std::string buttonName = b->getName();
        auto underscoreIndex = buttonName.find_last_of("_") + 1;
        int optionIndex = std::atoi(
            buttonName.substr(underscoreIndex, buttonName.length() - underscoreIndex).c_str()
        );

        std::u16string buttonText = b->getAttributeManager()->getText();
        MenuEvent* event = MenuEvent::create("click", target, this, optionIndex, buttonText);
        target->getEventDispatcher()->dispatchEvent(event);
        delete event;
    });

    //TODO style
    Font* font = Font::getFont(fontName, fontSize, window);
    int biggestWidth = 0;
    if (font != NULL) {
        std::u16string buttonText = b->getAttributeManager()->getText();
        font->getStringWidth(buttonText);
        biggestWidth = biggestWidth < minWidth ? minWidth : biggestWidth;
    }

    //Add button child and increment numOptions
    _addChild(b);
    numOptions++;

    //Set size for menu
    this->setAttribute(NUM_OPTIONS, numOptions);
    this->setAttribute(WIDTH_PERCENT, 0);
    this->setAttribute(HEIGHT_PERCENT, 0);
    this->setAttribute(WIDTH_OFFSET, biggestWidth + padding * 2 + borderWidth * 2);
    this->setAttribute(HEIGHT_OFFSET, optionHeight * numOptions + padding * 2 + borderWidth * 2);

    //Return the added button
    return b;
}

void oui::Menu::setTarget(Component* target) {
    this->target = target;
}

oui::Component* oui::Menu::getTarget() {
    return target;
}

void oui::Menu::setOptions(const std::vector<std::u16string>& options) {
    MenuAttributeManager* attributeManager = getAttributeManager();
    removeAllOptionComponents();
    std::vector<Button*> buttons;
    for (int i = (int) options.size() - 1; i >= 0; i--) {
        buttons.push_back(addOption(options.at(i), i));
    }
    
    int borderWidth = attributeManager->getBorderWidth();
    int padding = attributeManager->getPadding();
    int optionHeight = attributeManager->getOptionHeight();
    int biggestWidth = resetOptions();
    parseAttribute(SIZE, u"0 0 " + intToString(biggestWidth + padding * 2 + borderWidth * 2) + u" " + intToString(optionHeight * options.size() + padding * 2 + borderWidth * 2));
}

bool oui::Menu::removeOption(int index) {
    MenuAttributeManager* attributeManager = getAttributeManager();
    int numOptions = attributeManager->getNumOptions();
    if (index < 0 || index >= numOptions) {
        return false;
    }
    int padding = attributeManager->getPadding();
    int optionHeight = attributeManager->getOptionHeight();
    int borderWidth = attributeManager->getBorderWidth();
    Button* b2 = static_cast<Button*>(getChild("option_" + std::to_string(index)));
    b2->setAttribute(PERMANENT, false);
    removeChild(static_cast<Component*>(b2));
    int biggestWidth = resetOptions(index + 1);
    numOptions--;
    parseAttribute(SIZE, u"0 0 " + intToString(biggestWidth + padding * 2 + borderWidth * 2) + u" " + intToString(optionHeight * numOptions + padding * 2 + borderWidth * 2));
    return true;
}


int oui::Menu::resetOptions(int startIndex) {
    MenuAttributeManager* attributeManager = getAttributeManager();
    int borderWidth = attributeManager->getBorderWidth();
    int minWidth = attributeManager->getMinWidth();
    int fontSize = attributeManager->getFontSize();
    std::u16string font = attributeManager->getFontName();
    int numOptions = attributeManager->getNumOptions();
    int optionHeight = attributeManager->getOptionHeight();
    int padding = attributeManager->getPadding();

    int biggestWidth = minWidth;
    Font* f = Font::getFont(font, fontSize, window);
    for (int i = startIndex; i < numOptions; i++) {
        Button* b2 = static_cast<Button*>(getChild("option_" + std::to_string(i)));

        b2->setName("option_" + std::to_string(i));
        b2->parseAttribute(SIZE, u"100 0 " + intToString(-padding * 2 - borderWidth * 2) + u" " + intToString(optionHeight));
        b2->parseAttribute(LOCATION, u"0 0 " + intToString(padding + borderWidth) + u" " + intToString(optionHeight * (i) + padding + borderWidth));
        b2->parseAttribute(FONT, font + u" " + intToString(fontSize));
        b2->setAttribute(BORDER_STYLE, u"none");
        if (b2->getWidth() > biggestWidth) {
            std::u16string buttonText = b2->getAttributeManager()->getText();
            biggestWidth = f->getStringWidth(buttonText);
        }
    }
    return biggestWidth;
}

void oui::Menu::removeAllOptionComponents() {
    std::vector<Component*>::iterator it;
    for (it = children.begin(); it != children.end(); ) {
        delete *it;
        it = children.erase(it);
    }
}

oui::MenuAttributeManager* oui::Menu::getAttributeManager() {
    return static_cast<MenuAttributeManager*>(attributeManager);
}