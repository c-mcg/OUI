
#include "component/impl/label/OUI_Label.h"
#include "util/OUI_StringUtil.h"
#include "component/attribute/OUI_AttributeNames.h"

using namespace oui::AttributeNames;

oui::Label::~Label() {
}

oui::Label::Label(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher, LabelAttributeManager* attributeManager) : 
    Component("label", name, classes, false, eventDispatcher, attributeManager)
{
    setAttribute(TEXT, u"");
    parseAttribute(FONT, u"notoserif 12");
    setAttribute(BACKGROUND_COLOR_1, Color::NONE);
    setAttribute(BACKGROUND_COLOR_2, Color::NONE);
    setAttribute(TEXT_COLOR, Color::BLACK);
    setAttribute(AUTO_SIZE, true);
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