#ifndef OUI_CONTAINER_H
#define OUI_CONTAINER_H

#include "OUI_Export.h"
#include "comp/OUI_Component.h"

namespace oui {

    class Container: public Component {

        protected: StyleSheet* styleSheet;
        protected: std::vector<Component*> children;
        protected: std::vector<Component*> processableChildren;

        public: OUI_API ~Container();
        public: OUI_API Container(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing=false);

        public: OUI_API void onProcessableChildAdded(Component* addedChild);
        public: OUI_API void onProcessableChildRemoved(Component* addedChild);

        public: OUI_API void flagGraphicsUpdateAll();

        //Sets the style sheet and deletes the current one if it exists
        //Two container styleSheets should never point to the same sheet
        public: OUI_API void setStyleSheet(StyleSheet* sheet);
        public: OUI_API virtual Style* createStyle(StyleSheet* sheet = NULL) override;
        public: OUI_API virtual void setProfile(const std::u16string& profile) override;

        public: OUI_API virtual void addedToContainer(Container* container) override;

        public: OUI_API virtual void setSelected(bool selected) override;

        public: OUI_API virtual void redraw() override;

        public: OUI_API virtual void redrawChildren();

        public: OUI_API Component* getComponentAt(int x, int y);

        public: OUI_API int getIndexOf(const std::string& name);
        public: OUI_API int getIndexOf(Component* c);

        public: OUI_API bool isContainer() override;

        public: OUI_API virtual void removeAllChildren(bool shouldDelete = false);
        public: OUI_API Component* removeChild(const std::string& name, bool shouldDelete = false);
        public: OUI_API Component* removeChild(Component* child, bool shouldDelete = false);
        public: OUI_API virtual Component* removeChild(int index, bool shouldDelete = false);

        public: OUI_API int getNumChildren();
        public: OUI_API bool isDuplicateName(const std::string& name, Component* ignore = NULL);
        public: OUI_API virtual bool addChild(Component* child);//TODO make applying style optional in while adding child
        public: OUI_API Component* getChild(const std::string& name);
        public: OUI_API virtual Component* getChild(int index);
        public: OUI_API Container* getChildCont(const std::string& name);
        public: OUI_API Container* getChildCont(int index);

        public: OUI_API void setHovered(bool hovered) override;
        public: OUI_API void setMouseDown(bool mouseDown) override;


        public: OUI_API void addOSALStyle(const std::u16string& sheet);
        public: OUI_API void addOSALStyle(OSAL::Sheet sheet);

        //Adds a style sheet to the container
        //This is combined with existing style sheets
        //Style sheets are applied to children of the container
        public: OUI_API void addStyleSheet(StyleSheet* sheet);


        public: OUI_API virtual StyleSheet* getAllStyleSheets();

    };

}

#endif