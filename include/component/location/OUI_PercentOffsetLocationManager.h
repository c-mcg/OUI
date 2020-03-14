#ifndef OUI_PERCENT_OFFSET_LOCATION_MANAGER_H
#define OUI_PERCENT_OFFSET_LOCATION_MANAGER_H

#include "OUI_Export.h"
#include "component/location/OUI_LocationManager.h"

namespace oui {

    class OUI_API PercentOffsetLocationManager : public LocationManager {

        public:
            PercentOffsetLocationManager();

        protected:
            virtual int calculateRelativeX();
            virtual int calculateRelativeY();

    };

}

#endif