#ifndef OUI_H
#define OUI_H

#include "OUI_Export.h"
#include "util/OUI_StringUtil.h"
#include "window/OUI_window.h"
#include "components/button/OUI_Button.h"
#include "components/panel/OUI_Panel.h"
#include "components/drawablepanel/OUI_DrawablePanel.h"
#include "components/textfield/OUI_TextField.h"
#include "components/menu/OUI_Menu.h"
#include "components/scrollpanel/OUI_ScrollPanel.h"
#include "components/OUI_ComponentLoader.h"
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