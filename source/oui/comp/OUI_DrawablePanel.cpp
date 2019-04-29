
#include "oui/comp/OUI_DrawablePanel.h"
#include "OUI.h"//TODO this was for testing

oui::DrawablePanel::~DrawablePanel() {
}

oui::DrawablePanel::DrawablePanel(const std::string& name, const std::string& classes) : 
    draw{[](Graphics* g, Component* c) {}}, // No op
    Container("drawpanel", name, classes) {
    parseAttribute("bg-color", u"0 0 0 0");
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