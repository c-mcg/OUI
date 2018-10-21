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
		public: Container(std::string tag, std::string name, std::string classes, bool needsProcessing=false);

		public: void onProcessableChildAdded(Component* addedChild);
		public: void onProcessableChildRemoved(Component* addedChild);

		public: void flagGraphicsUpdateAll();

		//Sets the style sheet and deletes the current one if it exists
		//Two container styleSheets should never point to the same sheet
		public: void setStyleSheet(StyleSheet* sheet);
		public: virtual Style* createStyle(StyleSheet* sheet = NULL);
		public: virtual void setProfile(std::u16string profile);

		public: virtual void addedToContainer(Container* container);

		public: virtual void setSelected(bool selected);

		public: virtual void redraw();

		public: virtual void redrawChildren();

		public: Component* getComponentAt(int x, int y);

		public: int getIndexOf(std::string name);
		public: int getIndexOf(Component* c);

		public: bool isContainer();

		public: virtual void removeAllChildren(bool shouldDelete = false);
		public: Component* removeChild(std::string name, bool shouldDelete = false);
		public: Component* removeChild(Component* child, bool shouldDelete = false);
		public: virtual Component* removeChild(int index, bool shouldDelete = false);

		public: int getNumChildren();
		public: bool isDuplicateName(std::string name, Component* ignore = NULL);
		public: virtual bool addChild(Component* child);//TODO make applying style optional in while adding child
		public: Component* getChild(std::string name);
		public: virtual Component* getChild(int index);
		public: Container* getChildCont(std::string name);
		public: Container* getChildCont(int index);

		public: void setHovered(bool hovered);
		public: void setMouseDown(bool mouseDown);


		public: void addOSALStyle(std::u16string sheet);
		public: void addOSALStyle(OSAL::Sheet sheet);

		//Adds a style sheet to the container
		//This is combined with existing style sheets
		//Style sheets are applied to children of the container
		public: void addStyleSheet(StyleSheet* sheet);


		public: virtual oui::StyleSheet* getAllStyleSheets();

	};

}

#endif