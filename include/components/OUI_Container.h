#ifndef OUI_CONTAINER_H
#define OUI_CONTAINER_H

#include "OUI_Export.h"
#include "components/OUI_Component.h"
#include "components/OUI_ContainerAttributeManager.h"

namespace oui {

    class Container: public Component {

        public:
        
            OUI_API ~Container();
            OUI_API Container(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing=false, EventDispatcher* eventDispatcher=new EventDispatcher(), ContainerAttributeManager* attributeManager=new ContainerAttributeManager());

            OUI_API void onProcessableChildAdded(Component* addedChild);
            OUI_API void onProcessableChildRemoved(Component* addedChild);

            OUI_API void flagGraphicsUpdateAll();

            OUI_API virtual void addedToContainer(Container* container) override;

            OUI_API virtual void setSelected(bool selected) override;

            OUI_API virtual void redraw() override;

            OUI_API virtual void redrawChildren();

            OUI_API Component* getComponentAt(int x, int y);

            OUI_API int getIndexOf(const std::string& name);
            OUI_API int getIndexOf(Component* c);

            OUI_API bool isContainer() override;

            OUI_API virtual void removeAllChildren(bool shouldDelete = false);
            OUI_API Component* removeChild(const std::string& name, bool shouldDelete = false);
            OUI_API Component* removeChild(Component* child, bool shouldDelete = false);
            OUI_API virtual Component* removeChild(int index, bool shouldDelete = false);

            OUI_API int getNumChildren();
            OUI_API bool isDuplicateName(const std::string& name, Component* ignore = NULL);
            OUI_API virtual bool addChild(Component* child);
            OUI_API Component* getChild(const std::string& name);
            OUI_API virtual Component* getChild(int index);
            OUI_API Container* getChildCont(const std::string& name);
            OUI_API Container* getChildCont(int index);

            OUI_API void setHovered(bool hovered) override;
            OUI_API void setMouseDown(bool mouseDown) override;


            OUI_API void addOSALStyle(const std::u16string& sheet);
            OUI_API void addOSALStyle(OSAL::Sheet sheet);

            OUI_API virtual StyleSheet* getAllStyleSheets();

        protected:
        
            std::vector<Component*> children;
            std::vector<Component*> processableChildren;

        private: 
            ContainerAttributeManager* getAttributeManager();

    };

}

#endif