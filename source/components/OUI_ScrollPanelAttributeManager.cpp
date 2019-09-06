#include "components/OUI_ScrollPanelAttributeManager.h"
#include "components/OUI_ScrollPanel.h"

oui::ScrollPanelAttributeManager::ScrollPanelAttributeManager():
    ContainerAttributeManager()
{

}

void oui::ScrollPanelAttributeManager::setProfile(const std::u16string& profileName) {
    Graphics* graphics = component->getGraphics();
    int w = graphics->getWidth(), h = graphics->getHeight();

    ComponentAttributeManager::setProfile(profileName);

    AttributeProfile* profile = style->getProfile(profileName);
    if (profile != NULL) {
        
        //TODO replace SCROLLBAR_SIZE with scrollbar-size

    }

    if (w != graphics->getWidth() || h != graphics->getHeight()) {
        ScrollPanel* scrollPanel = static_cast<ScrollPanel*>(component);
        scrollPanel->handleResize();
    }

    ContainerAttributeManager::setProfile(profileName);
    
}
