
#include "oui/comp/OUI_Menu.h"
#include "util/OUI_StringUtil.h"

#include <algorithm>
#include <set>

oui::Menu::~Menu() {

}

oui::Menu::Menu(std::string name, std::string classes) : Container("menu", name, classes) {
	//setSize(0, 0, 150, 30);
	setAttribute("border-style", u"solid");
	setAttribute("border-color-r", 0);
	setAttribute("border-color-g", 0);
	setAttribute("border-color-b", 0);
	setAttribute("border-color-a", 128);
	parseAttribute("bg-color", u"225 225 225 255");

	parseAttribute("hover-color", u"200 200 200 255");
	setAttribute("padding", 4);
	setAttribute("min-width", 100);
	setAttribute("option-height", 20);
	parseAttribute("font", u"notoserif 14");

	parseAttribute("options", u"");
}

void oui::Menu::setProfile(std::u16string profileName) {
	AttributeProfile* profile = style->getProfile(profileName);
	if (profile != NULL) {

		hoverColor = Color(profile->getInt("hover-color-r"), profile->getInt("hover-color-g"), profile->getInt("hover-color-b"), profile->getInt("hover-color-a"));

		padding = profile->getInt("padding");

		minWidth = profile->getInt("min-width");

		optionHeight = profile->getInt("option-height");

		minWidth = profile->getInt("min-width");

		//Font
		font = profile->getString("font-face");
		fontSize = profile->getInt("font-size");

		//Options
		bool optionsChanged = false;
		std::vector<std::u16string> options;
		for (int i = 0; i < profile->getInt("options_length"); i++) {
			options.push_back(profile->getString("options_" + std::to_string(i)));
			if (i >= this->options.size() || options.at(i) != this->options.at(i)) {
				optionsChanged = true;
			}
		}
		if (optionsChanged) {
			setOptions(options);
			int borderWidth = profile->getInt("border-width");
			int biggestWidth = resetOptions();
			parseAttribute("size", u"0 0 " + intToString(biggestWidth + padding * 2 + borderWidth * 2) + u" " + intToString(optionHeight * numOptions + padding * 2 + borderWidth * 2));
		}
	}
	
	Container::setProfile(profileName);
}

bool oui::Menu::_addChild(Button* child) {
	return Container::addChild(child);
}

bool oui::Menu::addChild(Component* child) {
	std::cout << "You can't add children to menus, use addOption instead" << std::endl;
	return false;
}

oui::Button* oui::Menu::addOption(std::u16string option) {
	return addOption(option, numOptions);
}
oui::Button* oui::Menu::addOption(std::u16string option, int index) {
	AttributeProfile* profile = getCurrentProfile();

	//Create button for option
	Button* b = new Button("option_" + std::to_string(index), "menuOption");
	b->setAttribute("permanent", true);

	b->setAttribute("text", option);

	int borderWidth = profile->getInt("border-width");

	//Set size
	b->setAttribute("width-percent", 100);
	b->setAttribute("height-percent", 0);
	b->setAttribute("width-offset", -padding * 2 - borderWidth * 2);
	b->setAttribute("height-offset", optionHeight);

	//Set location
	b->setAttribute("x-percent", 0);
	b->setAttribute("y-percent", 0);
	b->setAttribute("x-offset", padding + borderWidth);
	b->setAttribute("y-offset", optionHeight * index + padding + borderWidth);

	//Font
	b->setAttribute("font-face", font);
	b->setAttribute("font-size", fontSize);

	b->setAttribute("border-style", u"none");
	b->setAttribute("center-text", false);

	//Apply hover colour
	Color c = hoverColor;
	b->setAttribute("bg-color1-r", c.getR(), u"hover");
	b->setAttribute("bg-color2-r", c.getR(), u"hover");
	b->setAttribute("bg-color1-g", c.getG(), u"hover");
	b->setAttribute("bg-color2-g", c.getG(), u"hover");
	b->setAttribute("bg-color1-b", c.getB(), u"hover");
	b->setAttribute("bg-color2-b", c.getB(), u"hover");

	//Add button click event handler
	b->addEventListener(Event::CLICKED, [](MouseEvent e, Component* c) {

		std::string name = c->getName();
		int numIndex = name.find_last_of("_") + 1;

		//Create event to pass to menu handler
		MenuEvent menuEvt = MenuEvent(std::atoi(name.substr(numIndex, name.length() - numIndex).c_str()), c->getCurrentProfile()->getString("text"));

		//Handle and dispose of menu event
		((Menu*) c->getParent())->handleEvent(e);
	});

	//TODO style
	Font* font = Font::getFont(this->font, fontSize, window);
	int biggestWidth = font->getStringWidth(b->getCurrentProfile()->getString("text"));
	biggestWidth = biggestWidth < minWidth ? minWidth : biggestWidth;

	//Reorganize options if inserted
	if (numOptions > 0) {

		//Rename button at desired index
		Button* currentButton = (Button*) getChild("option_" + std::to_string(index));
		currentButton->setName("MENU_TEMP_NAME_" + std::to_string(index));

		//Reorganize remaining items
		Button* nextButton;
		for (int i = index; i < numOptions; i++) {
			currentButton = (Button*) getChild("MENU_TEMP_NAME_" + std::to_string(i));
			nextButton = (Button*) getChild("option_" + std::to_string(i + 1));

			//Set next button to temp name
			if (nextButton != NULL) {
				nextButton->setName("MENU_TEMP_NAME_" + std::to_string(i + 1));
			}

			//Rename current button to next button's name
			currentButton->setName("option_" + std::to_string(i + 1));

			//Set size for button
			currentButton->setAttribute("width-percent", 100);
			currentButton->setAttribute("height-percent", 0);
			currentButton->setAttribute("width-offset", -padding * 2 - borderWidth * 2);
			currentButton->setAttribute("height-offset", optionHeight);

			//Set location
			currentButton->setAttribute("x-percent", 0);
			currentButton->setAttribute("y-percent", 0);
			currentButton->setAttribute("x-offset", padding + borderWidth);
			currentButton->setAttribute("y-offset", optionHeight * (i + 1) + padding + borderWidth);

			//Continue calculating biggest width
			if (currentButton->getWidth() > biggestWidth) {
				biggestWidth = font->getStringWidth(currentButton->getCurrentProfile()->getString("text"));
			}
		}

	}

	//Add button child and increment numOptions
	_addChild(b);
	numOptions++;

	//Set size for menu
	this->setAttribute("width-percent", 0);
	this->setAttribute("height-percent", 0);
	this->setAttribute("width-offset", biggestWidth + padding * 2 + borderWidth * 2);
	this->setAttribute("height-offset", optionHeight * numOptions + padding * 2 + borderWidth * 2);

	//Return the added button
	return b;
}


std::vector<oui::Button*> oui::Menu::addOptions(std::vector<std::u16string> options) {
	return addOptions(options, numOptions);
}

std::vector<oui::Button*> oui::Menu::addOptions(std::vector<std::u16string> options, int index) {
	std::vector<Button*> buttons;
	for (int i = options.size() - 1; i >= 0; i--) {
		buttons.push_back(addOption(options.at(i), index));
	}
	return buttons;
}

bool oui::Menu::removeOption(int index) {
	if (index < 0 || index >= numOptions) {
		return false;
	}
	int borderWidth = getCurrentProfile()->getInt("border-width");
	Button* b2 = (Button*) getChild("option_" + std::to_string(index));
	b2->setAttribute("permanent", false);
	removeChild((Component*) b2);
	int biggestWidth = resetOptions(index + 1);
	numOptions--;
	parseAttribute("size", u"0 0 " + intToString(biggestWidth + padding * 2 + borderWidth * 2) + u" " + intToString(optionHeight * numOptions + padding * 2 + borderWidth * 2));
	return true;
}
std::vector<oui::Button*> oui::Menu::setOptions(std::vector<std::u16string> options) {
	this->options = options;
	std::vector<Component*>::iterator it;
	for (it = children.begin(); it != children.end(); ) {
		delete *it;
		it = children.erase(it);
	}
	numOptions = 0;
	return addOptions(options);
}


int oui::Menu::resetOptions(int startIndex) {
	int borderWidth = getCurrentProfile()->getInt("border-width");
	int biggestWidth = minWidth;
	Font* f = Font::getFont(font, fontSize, window);
	for (int i = startIndex; i < numOptions; i++) {
		Button* b2 = (Button*) getChild("option_" + std::to_string(i));

		b2->setName("option_" + std::to_string(i));
		b2->parseAttribute("size", u"100 0 " + intToString(-padding * 2 - borderWidth * 2) + u" " + intToString(optionHeight));
		b2->parseAttribute("location", u"0 0 " + intToString(padding + borderWidth) + u" " + intToString(optionHeight * (i) + padding + borderWidth));
		b2->parseAttribute("font", font + u" " + intToString(fontSize));
		b2->setAttribute("border-style", u"none");
		if (b2->getWidth() > biggestWidth) {
			biggestWidth = f->getStringWidth(b2->getCurrentProfile()->getString("text"));
		}
	}
	return biggestWidth;
}
