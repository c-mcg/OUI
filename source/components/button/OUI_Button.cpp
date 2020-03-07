#include "window/OUI_window.h"
#include "components/container/OUI_Container.h"
#include "components/button/OUI_Button.h"
#include <iostream>
#include <sstream>
#include "attribute/OUI_AttributeNames.h"

using namespace oui::AttributeNames;


oui::Style* oui::Button::defaultStyle = Button::getDefaultButtonStyle();

oui::Style* oui::Button::getDefaultButtonStyle() {
    if (Button::defaultStyle == NULL) {
        Style* style = new Style();

        AttributeProfile* profile = new AttributeProfile("Button::defaultStyle['default']", Component::getDefaultComponentStyle()->getProfile(u"default"));

        profile->setAttribute(TEXT, u"");
        profile->setAttribute(IMAGE, u"");
        profile->setAttribute(FONT_FACE, u"notoserif");
        profile->setAttribute(FONT_SIZE, 14);
        profile->setAttribute(CURSOR, u"pointer");
        profile->setAttribute(BACKGROUND_COLOR_1, Color(220, 200, 220, 255));
        profile->setAttribute(BACKGROUND_COLOR_2, Color(220, 200, 220, 255));
        profile->setAttribute(BORDER_STYLE, u"solid");
        profile->setAttribute(BORDER_COLOR, Color(0, 0, 0, 64));
        profile->setAttribute(TEXT_COLOR, Color(24, 24, 24, 255));
        profile->setAttribute(WIDTH_PERCENT, 0);
        profile->setAttribute(HEIGHT_PERCENT, 0);
        profile->setAttribute(WIDTH_OFFSET, 125);
        profile->setAttribute(HEIGHT_OFFSET, 26);
        profile->setAttribute(CENTER_TEXT, true);

        AttributeProfile* hoverProfile = new AttributeProfile("Button::defaultStyle['hover']", Component::getDefaultComponentStyle()->getProfile(u"hover"));

        hoverProfile->setAttribute(BACKGROUND_COLOR_1, Color(230, 230, 230, 255));
        hoverProfile->setAttribute(BACKGROUND_COLOR_2, Color(230, 230, 230, 255));

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
}

void oui::Button::redraw() {
    ButtonAttributeManager* attributeManager = getAttributeManager();
    Component::redraw();

    Image* image = attributeManager->getImage();

    // Draw image
    if(image != NULL) {
        graphics->drawImage(image, getWidth() / 2 - image->getWidth() / 2, getHeight() / 2 - image->getHeight() / 2);
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

oui::ButtonAttributeManager* oui::Button::getAttributeManager() {
    return static_cast<ButtonAttributeManager*>(attributeManager);
}