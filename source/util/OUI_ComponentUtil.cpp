
#include "util/OUI_ComponentUtil.h"
#include "components/container/OUI_Container.h"

#include <iostream> // TODO remove

bool oui::util::isComponentChildOf(Component* child, Component* parent) {
    if (child == NULL || parent == NULL) {
        return false;
    }

    Component* directParent = (Component*) child->getParent(); 
    if (directParent != NULL) {
        return directParent == parent ? true : isComponentChildOf(directParent, parent);
    }
    return false;
}

bool oui::util::componentContainsPoint(Component* component, int localX, int localY) {
    int x = component->getX();
    int y = component->getY();
    int width = component->getWidth();
    int height = component->getHeight();
    return localX >= x && localX <= x + width &&
        localY >= y && localY <= y + height;
}

oui::Component* oui::util::getComponentAt(Container* container, int localX, int localY) {
    Component* foundComponent = NULL;
    int containerX = container->isWindow() ? 0 : container->getX();
    int containerY = container->isWindow() ? 0 : container->getY();
    auto children = container->getChildren();
    std::map<int, std::vector<Component*>> zIndexes = orderByZIndex(children);

    auto it = zIndexes.end();
    if (zIndexes.size() > 0) {
        it--;
        while(true) { // Iterate zindexes in reverse order
            std::vector<Component*> components = it->second;

            for(auto it2 = components.begin(); it2 != components.end(); it2++) {
                Component* component = *it2;
                int pointX = localX - containerX;
                int pointY = localY - containerY;
                bool componentVisible = component->getAttributeManager()->isVisible();
                bool componentInteractable = component->getAttributeManager()->isInteractable();
                if(componentVisible && componentInteractable &&
                    util::componentContainsPoint(component, pointX, pointY)) {
                    foundComponent = component;
                    if (component->isContainer()) {
                        Container* subContainer = static_cast<Container*>(component);
                        foundComponent = getComponentAt(subContainer, pointX, pointY);
                    }
                    goto break_outer_loop;
                }
            }

            if (it == zIndexes.begin()) {
                break;
            }

            it--;
        }
    }

break_outer_loop:
    return foundComponent != NULL ? foundComponent : static_cast<Component*>(container);
}

std::map<int, std::vector<oui::Component*>> oui::util::orderByZIndex(std::vector<oui::Component*> components) {
    std::map<int, std::vector<Component*>> zIndexes;

    for (auto it = components.begin(); it != components.end(); it++) {
        Component* child = *it;

        ComponentAttributeManager* childAttributeManager = child->getAttributeManager();
        int childZ = childAttributeManager->getZ();
        std::vector<Component*> compsForZIndex;
        auto zIt = zIndexes.find(childZ);
        if(zIt != zIndexes.end()) {
            compsForZIndex = zIt->second;
        }
        compsForZIndex.push_back(child);
        zIndexes.insert_or_assign(childZ, compsForZIndex);
    }

    // std::sort(zIndexes.begin(), zIndexes.end(), std::less<int>());
    return zIndexes;
}
