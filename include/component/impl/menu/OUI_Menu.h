#ifndef OUI_MENU_H
#define OUI_MENU_H

#include "OUI_Export.h"
#include "component/impl/container/OUI_Container.h"
#include "component/impl/button/OUI_Button.h"
#include "component/impl/menu/OUI_MenuAttributeManager.h"

namespace oui {

    class Menu : public Container {

        public:
        
            OUI_API ~Menu();
            OUI_API Menu(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher=new EventDispatcher(), MenuAttributeManager* attributeManager=new MenuAttributeManager());

            // Used to disable adding children traditionally
            OUI_API bool addChild(Component* child) override;

            OUI_API bool removeOption(int index);

            void setTarget(Component* component);
            Component* getTarget();

            // Start of internal public methods
            int resetOptions(int startIndex = 0);
            void removeAllOptionComponents();
            void setOptions(const std::vector<std::u16string>& options);

            OUI_API virtual MenuAttributeManager* getAttributeManager() override;

        private:

            Component* target;

            Button* addOption(const std::u16string& option, int index);

            // Used to override the disabled addChild above
            bool _addChild(Button* child);

    };
}
#endif
