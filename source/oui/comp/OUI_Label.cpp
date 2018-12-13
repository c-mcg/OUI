
#include "oui/comp/OUI_Label.h"
#include "util/OUI_StringUtil.h"

oui::Label::~Label() {
	font = NULL;
}

oui::Label::Label(const std::string& name, const std::string& classes) : 
	autoSize{true}, font{NULL},
	Component("label", name, classes) {
	setAttribute("text", u"");
	parseAttribute("font", u"notoserif 12");
	parseAttribute("bg-color", u"0 0 0 0");
	parseAttribute("text-color", u"0 0 0 255");
	setAttribute("auto-size", true);
}

void oui::Label::setProfile(const std::u16string& profileName) {

	AttributeProfile* profile = style->getProfile(profileName);
	if (profile != NULL) {

		//Auto-size
		autoSize = profile->getBool("auto-size");

		text = profile->getString("text");

		//Font
		font = Font::getFont(profile->getString("font-face"), profile->getInt("font-size"), window);

		//Text-color
		textColor = Color(profile->getInt("text-color-r"), profile->getInt("text-color-g"), profile->getInt("text-color-b"), profile->getInt("text-color-a"));

	}

	if (autoSize) {
		parseAttribute("size", u"0 0 " + intToString(font->getStringWidth(text)) + u" " + intToString(font->getStringHeight(text)));
	}

	Component::setProfile(profileName);
}

void oui::Label::redraw() {
	Component::redraw();
	graphics->setColor(textColor);
	graphics->setFont(font);
	// std::cout << "text=" << convertUTF16toUTF8(text).c_str() << std::endl;
	graphics->drawText(text, 0, 0);
}