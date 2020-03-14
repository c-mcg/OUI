#include "component/location/OUI_PercentOffsetLocationManager.h"
#include "component/impl/container/OUI_Container.h"
#include "component/OUI_ComponentAttributeManager.h"

oui::PercentOffsetLocationManager::PercentOffsetLocationManager() : LocationManager() {

}

int oui::PercentOffsetLocationManager::calculateRelativeX() {
    Container* parent = target->getParent();
    ComponentAttributeManager* attributeManager = target->getAttributeManager();

    int xPercent = attributeManager->getXPercent();
    int xOffset = attributeManager->getXOffset();
    int centeredX = attributeManager->getCenteredX();
    //TODO percent based on screen size for Window class
    auto offset = (int) (xOffset - (centeredX ? target->calculateWidth() / 2 : 0));
    if (parent == NULL) {
        return (int) offset;
    }
    return (int) (((float) (xPercent) / 100.0) * parent->calculateWidth() + offset);
    
}
int oui::PercentOffsetLocationManager::calculateRelativeY() {
    Container* parent = target->getParent();
    ComponentAttributeManager* attributeManager = target->getAttributeManager();

    int yPercent = attributeManager->getYPercent();
    int yOffset = attributeManager->getYOffset();
    int centeredY = attributeManager->getCenteredY();
    //TODO percent based on screen size for Window class
    auto offset = (int) (yOffset - (centeredY ? target->calculateHeight() / 2 : 0));
    if (parent == NULL) {
        return (int) offset;
    }
    return (int) (((float) (yPercent) / 100.0) * parent->calculateHeight() + offset); 
}