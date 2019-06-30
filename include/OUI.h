#ifndef OUI_H
#define OUI_H

#include "OUI_Export.h"
#include "util/OUI_StringUtil.h"
#include "OUI_Window.h"
#include "comp/OUI_Button.h"
#include "comp/OUI_Panel.h"
#include "comp/OUI_DrawablePanel.h"
#include "comp/OUI_TextField.h"
#include "comp/OUI_Menu.h"
#include "comp/OUI_ScrollPanel.h"
#include "comp/OUI_ComponentLoader.h"
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