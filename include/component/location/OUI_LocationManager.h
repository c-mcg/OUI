#ifndef OUI_COMPONENT_LOCATION_MANAGER_H
#define OUI_COMPONENT_LOCATION_MANAGER_H

#include "OUI_Export.h"

namespace oui {

    class Component;

    class OUI_API LocationManager {

        public:
            LocationManager(); // TODO optimize by taking array of strings for property names that should cause recalculations

            void setTarget(Component* component);

            int getRelativeX();
            int getRelativeY();
            int getWindowX();
            int getWindowY();

            void setScrollOffsetX(int scrollOffsetX);
            void setScrollOffsetY(int scrollOffsetY);

        protected:
            Component* target;

            virtual int calculateRelativeX();
            virtual int calculateRelativeY();

            int calculateWindowX();
            int calculateWindowY();

        private:
            int x;
            int y;
            int scrollOffsetX;
            int scrollOffsetY;

    };

}

#endif