
#include "oui/comp/OUI_DrawablePanel.h"
#include "OUI.h"//TODO this was for testing

oui::DrawablePanel::~DrawablePanel() {
}

oui::DrawablePanel::DrawablePanel(std::string name, std::string classes) : Container("drawpanel", name, classes) {
	this->draw = [](oui::Graphics* g, oui::Component* c) {};
	parseAttribute("bg-color", u"0 0 0 0");
}

void oui::DrawablePanel::setDraw(std::function<void(oui::Graphics*, Component*)> draw) {
	this->draw = draw;
}

void oui::DrawablePanel::redraw() {
	graphics->clear();
	draw(graphics, this);
	redrawChildren();
	flagGraphicsUpdate();
}