#include "component/impl/scrollpanel/OUI_ScrollPanelAttributeManager.h"
#include "component/impl/scrollpanel/OUI_ScrollPanel.h"

oui::ScrollPanelAttributeManager::ScrollPanelAttributeManager():
    ContainerAttributeManager()
{

}

void oui::ScrollPanelAttributeManager::refreshProfile() {
    Graphics* graphics = component->getGraphics();
    int w = graphics->getWidth(), h = graphics->getHeight();

    ComponentAttributeManager::refreshProfile();
        
    // TODO replace SCROLLBAR_SIZE with scrollbar-size

    if (w != graphics->getWidth() || h != graphics->getHeight()) {
        ScrollPanel* scrollPanel = static_cast<ScrollPanel*>(component);
        scrollPanel->handleResize();
    }

    ContainerAttributeManager::refreshProfile();
    
}
