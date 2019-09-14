#include "components/menu/OUI_Menu.h"

#include <algorithm>
#include <set>

#include "event/OUI_MenuEvent.h"
#include "util/OUI_StringUtil.h"

oui::Menu::~Menu() {

}

oui::Menu::Menu(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher, MenuAttributeManager* attributeManager) : 
    target{NULL},
    Container("menu", name, classes, false, eventDispatcher, attributeManager) {
    //setSize(0, 0, 150, 30);

    // TODO use default style like other components
    setAttribute("border-style", u"solid");
    setAttribute("border-color", Color(0, 0, 0, 128));
    setAttribute("bg-color1", Color::WHITE);
    setAttribute("bg-color2", Color::WHITE);

    setAttribute("hover-color", Color(200, 200, 200, 255));
    setAttribute("padding", 4);
    setAttribute("min-width", 100);
    setAttribute("option-height", 20);
    parseAttribute("font", u"notoserif 14");

    setAttribute("options", std::vector<Attribute>());
}



bool oui::Menu::_addChild(Button* child) {
    return Container::addChild(child);
}

bool oui::Menu::addChild(Component* child) {
    std::cout << "You can't add children to menus, use addOption instead" << std::endl;
    return false;
}

oui::Button* oui::Menu::addOption(const std::u16string& option) {
    MenuAttributeManager* attributeManager = getAttributeManager();
    int numOptions = attributeManager->getNumOptions();
    return addOption(option, numOptions);
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
    b->setAttribute("permanent", true);

    b->setAttribute("text", option);

    int borderWidth = attributeManager->getBorderWidth();

    //Set size
    b->setAttribute("width-percent", 100);
    b->setAttribute("height-percent", 0);
    b->setAttribute("width-offset", -padding * 2 - borderWidth * 2);
    b->setAttribute("height-offset", optionHeight);

    //Set location
    b->setAttribute("x-percent", 0);
    b->setAttribute("y-percent", 0);
    b->setAttribute("x-offset", padding + borderWidth);
    b->setAttribute("y-offset", optionHeight * index + padding + borderWidth);

    //Font
    b->setAttribute("font-face", fontName);
    b->setAttribute("font-size", fontSize);

    b->setAttribute("border-style", u"none");
    b->setAttribute("center-text", false);

    //Apply hover colour
    Color c = hoverColor;
    b->setAttribute("bg-color1", c, u"hover");
    b->setAttribute("bg-color2", c, u"hover");

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

    //Reorganize options if inserted
    if (numOptions > 0) {

        //Rename button at desired index
        Button* currentButton = static_cast<Button*>(getChild("option_" + std::to_string(index)));
        currentButton->setName("MENU_TEMP_NAME_" + std::to_string(index));

        //Reorganize remaining items
        for (int i = index; i < numOptions; i++) {
            currentButton = static_cast<Button*>(getChild("MENU_TEMP_NAME_" + std::to_string(i)));
            Button* nextButton = static_cast<Button*>(getChild("option_" + std::to_string(i + 1)));

            //Set next button to temp name
            if (nextButton != NULL) {
                nextButton->setName("MENU_TEMP_NAME_" + std::to_string(i + 1));
            }

            //Rename current button to next button's name
            currentButton->setName("option_" + std::to_string(i + 1));

            //Set size for button
            currentButton->setAttribute("width-percent", 100);
            currentButton->setAttribute("height-percent", 0);
            currentButton->setAttribute("width-offset", -padding * 2 - borderWidth * 2);
            currentButton->setAttribute("height-offset", optionHeight);

            //Set location
            currentButton->setAttribute("x-percent", 0);
            currentButton->setAttribute("y-percent", 0);
            currentButton->setAttribute("x-offset", padding + borderWidth);
            currentButton->setAttribute("y-offset", optionHeight * (i + 1) + padding + borderWidth);

            //Continue calculating biggest width
            if (currentButton->getWidth() > biggestWidth) {
                std::u16string currentButtonText = currentButton->getAttributeManager()->getText();
                biggestWidth = font->getStringWidth(currentButtonText);
            }
        }

    }

    //Add button child and increment numOptions
    _addChild(b);
    numOptions++;

    //Set size for menu
    this->setAttribute("num-options", numOptions);
    this->setAttribute("width-percent", 0);
    this->setAttribute("height-percent", 0);
    this->setAttribute("width-offset", biggestWidth + padding * 2 + borderWidth * 2);
    this->setAttribute("height-offset", optionHeight * numOptions + padding * 2 + borderWidth * 2);

    //Return the added button
    return b;
}

void oui::Menu::setTarget(Component* target) {
    this->target = target;
}

oui::Component* oui::Menu::getTarget() {
    return target;
}

std::vector<oui::Button*> oui::Menu::addOptions(const std::vector<std::u16string>& options) {
    MenuAttributeManager* attributeManager = getAttributeManager();
    int numOptions = attributeManager->getNumOptions();
    return addOptions(options, numOptions);
}

std::vector<oui::Button*> oui::Menu::addOptions(const std::vector<std::u16string>& options, int index) {
    std::vector<Button*> buttons;
    for (int i = (int) options.size() - 1; i >= 0; i--) {
        buttons.push_back(addOption(options.at(i), index));
    }
    return buttons;
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
    b2->setAttribute("permanent", false);
    removeChild(static_cast<Component*>(b2));
    int biggestWidth = resetOptions(index + 1);
    numOptions--;
    parseAttribute("size", u"0 0 " + intToString(biggestWidth + padding * 2 + borderWidth * 2) + u" " + intToString(optionHeight * numOptions + padding * 2 + borderWidth * 2));
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
        b2->parseAttribute("size", u"100 0 " + intToString(-padding * 2 - borderWidth * 2) + u" " + intToString(optionHeight));
        b2->parseAttribute("location", u"0 0 " + intToString(padding + borderWidth) + u" " + intToString(optionHeight * (i) + padding + borderWidth));
        b2->parseAttribute("font", font + u" " + intToString(fontSize));
        b2->setAttribute("border-style", u"none");
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