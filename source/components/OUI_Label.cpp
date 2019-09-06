
#include "components/OUI_Label.h"
#include "util/OUI_StringUtil.h"

oui::Label::~Label() {
}

oui::Label::Label(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher, LabelAttributeManager* attributeManager) : 
    Component("label", name, classes, false, eventDispatcher, attributeManager)
{
    setAttribute("text", u"");
    parseAttribute("font", u"notoserif 12");
    parseAttribute("bg-color", u"0 0 0 0");
    parseAttribute("text-color", u"0 0 0 255");
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