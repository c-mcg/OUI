
#include "components/drawablepanel/OUI_DrawablePanel.h"
#include "attribute/OUI_AttributeNames.h"

using namespace oui::AttributeNames;

oui::DrawablePanel::~DrawablePanel() {
}

oui::DrawablePanel::DrawablePanel(const std::string& name, const std::string& classes) : 
    draw{[](Graphics* g, Component* c) {}}, // No op
    Container("drawpanel", name, classes) {
    setAttribute(BACKGROUND_COLOR_1, Color::NONE);
    setAttribute(BACKGROUND_COLOR_2, Color::NONE);
}

void oui::DrawablePanel::setDraw(std::function<void(Graphics*, Component*)> draw) {
    this->draw = draw;
}

void oui::DrawablePanel::redraw() {
    graphics->clear();
    draw(graphics, this);
    redrawChildren();
    flagGraphicsUpdate();
}