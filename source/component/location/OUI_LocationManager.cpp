#include "component/impl/container/OUI_Container.h"
#include "component/location/OUI_LocationManager.h"

oui::LocationManager::LocationManager() :
    x{0}, y{0}, scrollOffsetX{0}, scrollOffsetY{0} {
    
}

void oui::LocationManager::setTarget(Component* target) {
    this->target = target;
}

int oui::LocationManager::getRelativeX() {
    x = calculateRelativeX() - scrollOffsetX;
    return x;
}
int oui::LocationManager::getRelativeY() {
    y = calculateRelativeY() - scrollOffsetY;
    return y;
}
int oui::LocationManager::getWindowX() {
    return calculateWindowX();
}
int oui::LocationManager::getWindowY() {
    return calculateWindowY();
}


void oui::LocationManager::setScrollOffsetX(int scrollOffsetX) {
    this->scrollOffsetX = scrollOffsetX;
}
void oui::LocationManager::setScrollOffsetY(int scrollOffsetY) {
    this->scrollOffsetY = scrollOffsetY;
}

int oui::LocationManager::calculateRelativeX() {
    return 0;
}
int oui::LocationManager::calculateRelativeY() {
    return 0;
}


int oui::LocationManager::calculateWindowX() {
    Container* parent = target->getParent();
    if (parent == NULL) {
        return getRelativeX();
    }

    LocationManager* parentLocationManager = target->getParent()->getLocationManager();
    return getRelativeX() + parentLocationManager->getRelativeX();
}
int oui::LocationManager::calculateWindowY() {
    Container* parent = target->getParent();
    if (parent == NULL) {
        return getRelativeY();
    }

    LocationManager* parentLocationManager = target->getParent()->getLocationManager();
    return getRelativeY() +  parentLocationManager->getRelativeY();
}