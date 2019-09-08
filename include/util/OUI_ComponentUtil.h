#ifndef OUI_COMPONENT_UTIL_H
#define OUI_COMPONENT_UTIL_H

#include "OUI_Export.h"

#include <map>

namespace oui {

    class Component;
    class Container;

    namespace util {

        OUI_API bool isComponentChildOf(Component* child, Component* parent);

        OUI_API bool componentContainsPoint(Component* component, int localX, int localY);

        OUI_API Component* getComponentAt(Container* container, int x, int y);

        OUI_API std::map<int, std::vector<Component*>> orderByZIndex(std::vector<Component*> components);

    }

}

#endif