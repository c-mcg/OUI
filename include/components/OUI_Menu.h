#ifndef OUI_MENU_H
#define OUI_MENU_H

#include "OUI_Export.h"
#include "components/OUI_Container.h"
#include "components/OUI_Button.h"
#include "components/OUI_MenuAttributeManager.h"

namespace oui {

    class Menu : public Container {

        public:
        
            OUI_API ~Menu();
            OUI_API Menu(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher=new EventDispatcher(), MenuAttributeManager* attributeManager=new MenuAttributeManager());

            // Used to disable adding children traditionally
            OUI_API bool addChild(Component* child) override;

            OUI_API bool removeOption(int index);
            OUI_API std::vector<Button*> setOptions(const std::vector<std::u16string>&  options);

            void setTarget(Component* component);
            Component* getTarget();

            // Start of internal public methods
            int resetOptions(int startIndex = 0);
            void removeAllOptionComponents();
            std::vector<Button*> addOptions(const std::vector<std::u16string>& options);

        private:
            Component* target;

            MenuAttributeManager* getAttributeManager();

            Button* addOption(const std::u16string& option);
            Button* addOption(const std::u16string& option, int index);
            std::vector<Button*> addOptions(const std::vector<std::u16string>& options, int index);

            // Used to override the disabled addChild above
            bool _addChild(Button* child);

    };
}
#endif
