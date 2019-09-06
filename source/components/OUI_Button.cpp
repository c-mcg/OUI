#include "OUI_Window.h"
#include "components/OUI_Container.h"
#include "components/OUI_Button.h"
#include <iostream>
#include <sstream>

oui::Style* oui::Button::defaultStyle = Button::getDefaultButtonStyle();

oui::Style* oui::Button::getDefaultButtonStyle() {
    if (Button::defaultStyle == NULL) {
        Style* style = new Style();

        AttributeProfile* profile = new AttributeProfile("Button::defaultStyle['default']", Component::getDefaultComponentStyle()->getProfile(u"default"));

        profile->setAttribute("text", u"");
        profile->setAttribute("image", u"");
        profile->setAttribute("font-face", u"notoserif");
        profile->setAttribute("font-size", 14);
        profile->setAttribute("cursor", u"pointer");
        profile->setAttribute("bg-color1-r", 220);
        profile->setAttribute("bg-color1-g", 220);
        profile->setAttribute("bg-color1-b", 220);
        profile->setAttribute("bg-color1-a", 255);
        profile->setAttribute("bg-color2-r", 220);
        profile->setAttribute("bg-color2-g", 220);
        profile->setAttribute("bg-color2-b", 220);
        profile->setAttribute("bg-color2-a", 255);
        profile->setAttribute("border-style", u"solid");
        profile->setAttribute("border-color-r", 0);
        profile->setAttribute("border-color-g", 0);
        profile->setAttribute("border-color-b", 0);
        profile->setAttribute("border-color-a", 64);
        profile->setAttribute("text-color-r", 24);
        profile->setAttribute("text-color-g", 24);
        profile->setAttribute("text-color-b", 24);
        profile->setAttribute("text-color-a", 255);
        profile->setAttribute("width-percent", 0);
        profile->setAttribute("height-percent", 0);
        profile->setAttribute("width-offset", 125);
        profile->setAttribute("height-offset", 26);
        profile->setAttribute("center-text", true);

        AttributeProfile* hoverProfile = new AttributeProfile("Button::defaultStyle['hover']", Component::getDefaultComponentStyle()->getProfile(u"hover"));

        hoverProfile->setAttribute("bg-color1-r", 230);
        hoverProfile->setAttribute("bg-color1-g", 230);
        hoverProfile->setAttribute("bg-color1-b", 230);
        hoverProfile->setAttribute("bg-color1-a", 255);
        hoverProfile->setAttribute("bg-color2-r", 230);
        hoverProfile->setAttribute("bg-color2-g", 230);
        hoverProfile->setAttribute("bg-color2-b", 230);
        hoverProfile->setAttribute("bg-color2-a", 255);

        style->addProfile(u"default", profile);
        style->addProfile(u"hover", hoverProfile);

        Button::defaultStyle = style;
    }

    return Button::defaultStyle;
}

oui::Button::~Button() {
    //TODO
}

oui::Button::Button(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher, ButtonAttributeManager* attributeManager) :
    image{NULL},
    Component("button", name, classes, false, eventDispatcher, attributeManager)
{
    eventDispatcher->addEventListener("click", [this](ComponentEvent* event) {
        std::u16string link = this->getAttributeManager()->getLink();
		if (this->window == NULL || link.compare(u"") == 0) {
			return;
		}
		this->window->setPage(link);
	});
}


void oui::Button::addedToContainer(Container* parent) {
    ButtonAttributeManager* attributeManager = getAttributeManager();
    Component::addedToContainer(parent);
    if(window != NULL) {
        std::u16string imageString = attributeManager->getImageString();
        if(image == NULL && imageString.length() > 0) {
            image = Image::loadImage(imageString, window);
            flagGraphicsUpdate();
        }
    }
}

void oui::Button::redraw() {
    ButtonAttributeManager* attributeManager = getAttributeManager();
    Component::redraw();

    // Draw image
    if(image != NULL) {
        if(isHovered()) {
            //TODO
        } else {
            graphics->drawImage(getImage(), getWidth() / 2 - image->getWidth() / 2, getHeight() / 2 - image->getHeight() / 2);
        }
    }

    // Draw text
    std::u16string text = attributeManager->getText();
    Color textColor = attributeManager->getTextColor();
    Font* font = attributeManager->getFont();
    bool centerText = attributeManager->isTextCentered();
    if (text != u"") {
        graphics->setColor(textColor);
        graphics->setFont(font);
        graphics->drawTextLine(text, centerText ? getWidth() / 2 - font->getStringWidth(text) / 2 : 0, getHeight() / 2 - font->getStringHeight(text) / 2 - 1);
    }

    // Draw border overtop of everything
    drawBorder();
}

oui::Image* oui::Button::getImage() {
    return image;
}

oui::ButtonAttributeManager* oui::Button::getAttributeManager() {
    return static_cast<ButtonAttributeManager*>(attributeManager);
}