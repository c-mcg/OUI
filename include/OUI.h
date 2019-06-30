#ifndef OUI_H
#define OUI_H

#include "OUI_Export.h"
#include "util/OUI_StringUtil.h"
#include "OUI_Window.h"
#include "components/OUI_Button.h"
#include "components/OUI_Panel.h"
#include "components/OUI_DrawablePanel.h"
#include "components/OUI_TextField.h"
#include "components/OUI_Menu.h"
#include "components/OUI_ScrollPanel.h"
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