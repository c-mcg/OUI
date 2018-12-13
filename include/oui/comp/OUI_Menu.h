#ifndef OUI_MENU_H
#define OUI_MENU_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Container.h"
#include "oui/comp/OUI_Button.h"

namespace oui {

	class OUI_API Menu : public Container {

		private: int numOptions;//TODO this is harder than it looks to remove
		private: std::vector<std::u16string> options;
		
		//TODO none of these work get assigned through profiles
		private: std::u16string font;
		private: int fontSize;
		private: int minWidth;
		private: int optionHeight;
		private: Color hoverColor;
		private: int padding;
		

		public: ~Menu();
		public: Menu(const std::string& name, const std::string& classes);

		public: void setProfile(const std::u16string& profile) override;

		//Used to disable adding children traditionally
		public: bool addChild(Component* child) override;

		//Used to override the disabled addChild above
		private: bool _addChild(Button* child);

		//TODO remove these
		private: Button* addOption(const std::u16string& option);
		private: Button* addOption(const std::u16string& option, int index);
		private: std::vector<Button*> addOptions(const std::vector<std::u16string>& options);
		private: std::vector<Button*> addOptions(const std::vector<std::u16string>& options, int index);
		public: bool removeOption(int index);
		public: std::vector<Button*> setOptions(const std::vector<std::u16string>&  options);

		/*Returns the width of the longest string*/
		private: int resetOptions(int startIndex = 0);

	};
}
#endif
