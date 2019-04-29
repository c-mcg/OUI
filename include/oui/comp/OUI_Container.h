#ifndef OUI_CONTAINER_H
#define OUI_CONTAINER_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Component.h"

namespace oui {

    class OUI_API Container: public Component {

        protected: StyleSheet* styleSheet;
        protected: std::vector<Component*> children;
        protected: std::vector<Component*> processableChildren;

        public: ~Container();
        public: Container(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing=false);

        public: void onProcessableChildAdded(Component* addedChild);
        public: void onProcessableChildRemoved(Component* addedChild);

        public: void flagGraphicsUpdateAll();

        //Sets the style sheet and deletes the current one if it exists
        //Two container styleSheets should never point to the same sheet
        public: void setStyleSheet(StyleSheet* sheet);
        public: virtual Style* createStyle(StyleSheet* sheet = NULL) override;
        public: virtual void setProfile(const std::u16string& profile) override;

        public: virtual void addedToContainer(Container* container) override;

        public: virtual void setSelected(bool selected) override;

        public: virtual void redraw() override;

        public: virtual void redrawChildren();

        public: Component* getComponentAt(int x, int y);

        public: int getIndexOf(const std::string& name);
        public: int getIndexOf(Component* c);

        public: bool isContainer() override;

        public: virtual void removeAllChildren(bool shouldDelete = false);
        public: Component* removeChild(const std::string& name, bool shouldDelete = false);
        public: Component* removeChild(Component* child, bool shouldDelete = false);
        public: virtual Component* removeChild(int index, bool shouldDelete = false);

        public: int getNumChildren();
        public: bool isDuplicateName(const std::string& name, Component* ignore = NULL);
        public: virtual bool addChild(Component* child);//TODO make applying style optional in while adding child
        public: Component* getChild(const std::string& name);
        public: virtual Component* getChild(int index);
        public: Container* getChildCont(const std::string& name);
        public: Container* getChildCont(int index);

        public: void setHovered(bool hovered) override;
        public: void setMouseDown(bool mouseDown) override;


        public: void addOSALStyle(const std::u16string& sheet);
        public: void addOSALStyle(OSAL::Sheet sheet);

        //Adds a style sheet to the container
        //This is combined with existing style sheets
        //Style sheets are applied to children of the container
        public: void addStyleSheet(StyleSheet* sheet);


        public: virtual StyleSheet* getAllStyleSheets();

    };

}

#endif