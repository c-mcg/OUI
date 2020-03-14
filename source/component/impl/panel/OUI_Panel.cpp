
#include "component/impl/panel/OUI_Panel.h"

oui::Panel::~Panel() {

}

oui::Panel::Panel(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher, ContainerAttributeManager* attributeManager) :
    Container("panel", name, classes, false, eventDispatcher, attributeManager)
{

}