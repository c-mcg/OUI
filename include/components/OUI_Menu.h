#ifndef OUI_MENU_H
#define OUI_MENU_H

#include "OUI_Export.h"
#include "components/OUI_Container.h"
#include "components/OUI_Button.h"

namespace oui {

    class Menu : public Container {

        private: int numOptions;//TODO this is harder than it looks to remove
        private: std::vector<std::u16string> options;
        
        //TODO none of these work get assigned through profiles
        private: std::u16string font;
        private: int fontSize;
        private: int minWidth;
        private: int optionHeight;
        private: Color hoverColor;
        private: int padding;
        

        public: OUI_API ~Menu();
        public: OUI_API Menu(const std::string& name, const std::string& classes);

        public: OUI_API void setProfile(const std::u16string& profile) override;

        //Used to disable adding children traditionally
        public: OUI_API bool addChild(Component* child) override;

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
