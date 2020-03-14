#ifndef OUI_H
#define OUI_H

#include "OUI_Export.h"
#include "util/OUI_StringUtil.h"
#include "window/OUI_window.h"
#include "component/impl/button/OUI_Button.h"
#include "component/impl/panel/OUI_Panel.h"
#include "component/impl/drawablepanel/OUI_DrawablePanel.h"
#include "component/impl/textfield/OUI_TextField.h"
#include "component/impl/menu/OUI_Menu.h"
#include "component/impl/scrollpanel/OUI_ScrollPanel.h"
#include "component/OUI_ComponentLoader.h"
#include "OUI_Context.h"

#include "os/OUI_OS.h"


namespace oui {

    int OUI_API initialize();
    int OUI_API shutdown();
    void OUI_API sleep(int milliseconds);
    long long OUI_API currentTimeMillis();
    long long OUI_API currentTimeNanos();

    OUI_API OperatingSystem* OS();

}


#endif