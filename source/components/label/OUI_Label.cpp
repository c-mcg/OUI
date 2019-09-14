
#include "components/label/OUI_Label.h"
#include "util/OUI_StringUtil.h"

oui::Label::~Label() {
}

oui::Label::Label(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher, LabelAttributeManager* attributeManager) : 
    Component("label", name, classes, false, eventDispatcher, attributeManager)
{
    setAttribute("text", u"");
    parseAttribute("font", u"notoserif 12");
    setAttribute("bg-color1", Color::NONE);
    setAttribute("bg-color2", Color::NONE);
    setAttribute("text-color", Color::BLACK);
    setAttribute("auto-size", true);
}

void oui::Label::redraw() {
    LabelAttributeManager* attributeManager = getAttributeManager();
    Color textColor = attributeManager->getTextColor();
    Font* font = attributeManager->getFont();
    std::u16string text = attributeManager->getText();

    Component::redraw();

    graphics->setColor(textColor);
    graphics->setFont(font);
    graphics->drawText(text, 0, 0);
}

oui::LabelAttributeManager* oui::Label::getAttributeManager() {
    return static_cast<LabelAttributeManager*>(attributeManager);
}