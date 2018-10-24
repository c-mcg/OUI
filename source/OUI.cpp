#include "OUI.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <thread>
#include "oui/attribute/OUI_AttributeSubstitution.h"

#define _DEBUG

int oui::initialize() {
	if((SDL_Init(SDL_INIT_EVERYTHING)==-1)) { 
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

	ComponentLoader::addTag("button", [](std::string name, std::string classes, std::vector<std::string>, std::vector<std::u16string>) {
		return new oui::Button(name, classes);
	});

	ComponentLoader::addTag("panel", [](std::string name, std::string classes, std::vector<std::string>, std::vector<std::u16string>) {
		return new oui::Panel(name, classes);
	});

	ComponentLoader::addTag("drawpanel", [](std::string name, std::string classes, std::vector<std::string>, std::vector<std::u16string>) {
		return new oui::DrawablePanel(name, classes);
	});

	ComponentLoader::addTag("label", [](std::string name, std::string classes, std::vector<std::string>, std::vector<std::u16string>) {
		return new oui::Label(name, classes);
	});

	ComponentLoader::addTag("textfield", [](std::string name, std::string classes, std::vector<std::string>, std::vector<std::u16string>) {
		return new oui::TextField(name, classes);
	});

	ComponentLoader::addTag("men", [](std::string name, std::string classes, std::vector<std::string>, std::vector<std::u16string>) {
		return new oui::Menu(name, classes);
	});

	ComponentLoader::addTag("scrollpanel", [](std::string name, std::string classes, std::vector<std::string>, std::vector<std::u16string>) {
		return new oui::ScrollPanel(name, classes);
	});

	//Component
	AttributeSubstitution::addSubstitution("size", {"width-percent", "height-percent", "width-offset", "height-offset"});
	AttributeSubstitution::addSubstitution("location", {"x-percent", "y-percent", "x-offset", "y-offset"});
	AttributeSubstitution::addSubstitution("centered", std::vector<std::string>({"centered-x", "centered-y"}), AttributeSubstitution::APPLY_WHOLE);
	AttributeSubstitution::addSubstitution("bg-color", std::vector<std::string>({"bg-color1", "bg-color2"}), AttributeSubstitution::APPLY_WHOLE);
	AttributeSubstitution::addSubstitution("bg-color1", {"bg-color1-r", "bg-color1-g", "bg-color1-b", "bg-color1-a"});
	AttributeSubstitution::addSubstitution("bg-color2", {"bg-color2-r", "bg-color2-g", "bg-color2-b", "bg-color2-a"});
	AttributeSubstitution::addSubstitution("bg-color-r", std::vector<std::string>({"bg-color1-r", "bg-color2-r"}), AttributeSubstitution::APPLY_WHOLE);
	AttributeSubstitution::addSubstitution("bg-color-g", std::vector<std::string>({"bg-color1-g", "bg-color2-g"}), AttributeSubstitution::APPLY_WHOLE);
	AttributeSubstitution::addSubstitution("bg-color-b", std::vector<std::string>({"bg-color1-b", "bg-color2-b"}), AttributeSubstitution::APPLY_WHOLE);
	AttributeSubstitution::addSubstitution("bg-color-a", std::vector<std::string>({"bg-color1-a", "bg-color2-a"}), AttributeSubstitution::APPLY_WHOLE);
	AttributeSubstitution::addSubstitution("border-color", {"border-color-r", "border-color-g", "border-color-b", "border-color-a"});
	AttributeSubstitution::addSubstitution("right-click-options", "right-click-options", AttributeSubstitution::APPLY_INDEXED);

	//FONT (button, label, textfield)
	AttributeSubstitution::addSubstitution("font", std::vector<std::string>({"font-face", "font-size"}));
	AttributeSubstitution::addSubstitution("text-color", {"text-color-r", "text-color-g", "text-color-b", "text-color-a"});

	//Menu
	AttributeSubstitution::addSubstitution("hover-color", {"hover-color-r", "hover-color-g", "hover-color-b", "hover-color-a"});
	AttributeSubstitution::addSubstitution("options", "options", AttributeSubstitution::APPLY_INDEXED);

	//TextField
	AttributeSubstitution::addSubstitution("carat-color", {"carat-color-r", "carat-color-g", "carat-color-b", "carat-color-a"});
	AttributeSubstitution::addSubstitution("highlight-color", {"highlight-color-r", "highlight-color-g", "highlight-color-b", "highlight-color-a"});

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