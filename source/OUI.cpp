#include "OUI.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <thread>
#include "attribute/OUI_AttributeSubstitution.h"
#include "os/OUI_OS.h"

#ifdef __linux__//LINUX
    #include "os/linux/OUI_Linux.h"
#elif defined __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_MAC//MAC
    #endif
#elif defined _WIN32 || defined _WIN64//WINDOWS
    #include "os/windows/OUI_Windows.h"
#endif


oui::OperatingSystem* oui::OS() {
    #ifdef __linux__//LINUX

        return new LinuxOS();

    #elif defined __APPLE__

        #include "TargetConditionals.h"
        #ifdef TARGET_OS_MAC//MAC

            return new OSX();

        #endif

    #elif defined _WIN32 || defined _WIN64//WINDOWS

        return new WindowsOS();

    #endif
}


// #define _DEBUG

int oui::initialize() {

    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)==-1)) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        return false;
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        return false;
    }
    
    //Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        return false;
    }

    ComponentLoader::addTag("button", [](const std::string& name, const std::string& classes, std::vector<std::string>, std::vector<std::u16string>) {
        return new Button(name, classes);
    });

    ComponentLoader::addTag("panel", [](const std::string& name, const std::string& classes, std::vector<std::string>, std::vector<std::u16string>) {
        return new Panel(name, classes);
    });

    ComponentLoader::addTag("drawpanel", [](const std::string& name, const std::string& classes, std::vector<std::string>, std::vector<std::u16string>) {
        return new DrawablePanel(name, classes);
    });

    ComponentLoader::addTag("label", [](const std::string& name, const std::string& classes, std::vector<std::string>, std::vector<std::u16string>) {
        return new Label(name, classes);
    });

    ComponentLoader::addTag("textfield", [](const std::string& name, const std::string& classes, std::vector<std::string>, std::vector<std::u16string>) {
        return new TextField(name, classes);
    });

    ComponentLoader::addTag("menu", [](const std::string& name, const std::string& classes, std::vector<std::string>, std::vector<std::u16string>) {
        return new Menu(name, classes);
    });

    ComponentLoader::addTag("scrollpanel", [](const std::string& name, const std::string& classes, std::vector<std::string>, std::vector<std::u16string>) {
        return new ScrollPanel(name, classes);
    });

    //Component
    AttributeSubstitution::addSubstitution("size", {"width-percent", "height-percent", "width-offset", "height-offset"});
    AttributeSubstitution::addSubstitution("location", {"x-percent", "y-percent", "x-offset", "y-offset"});
    AttributeSubstitution::addSubstitution("centered", std::vector<std::string>({"centered-x", "centered-y"}), AttributeSubstitution::APPLY_WHOLE);
    AttributeSubstitution::addSubstitution("bg-color", std::vector<std::string>({"bg-color1", "bg-color2"}), AttributeSubstitution::APPLY_WHOLE);

    //FONT (button, label, textfield)
    AttributeSubstitution::addSubstitution("font", std::vector<std::string>({"font-face", "font-size"}));

    OS()->initialize();

    return 0;
}

int oui::shutdown() {
    SDL_Quit();
    return 0;
}

void oui::sleep(int milliseconds) {
     std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

long long oui::currentTimeMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
}

long long oui::currentTimeNanos() {
    return std::chrono::duration_cast<std::chrono::nanoseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
}