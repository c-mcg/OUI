#ifndef OUI_COMPONENT_H
#define OUI_COMPONENT_H

#include "OUI_Export.h"
#include "oui/event/OUI_Event.h"
#include "oui/gfx/OUI_Graphics.h"
#include <vector>
#include <functional>
#include "oui/attribute/OUI_StyleSheet.h"

namespace oui {

	class Window;
	class Container;

	struct Bounds {
		int top, bottom;
		int left, right;
	};

	class OUI_API Component {

/* START OF VARIABLES */

		public: const bool needsProcessing;

	//Constants
		public: static const char BORDER_NONE = 0;//TODO change to enum
		public: static const char BORDER_SOLID = 1;
    
		private: static Style* defaultStyle;
		public: static Style* getDefaultComponentStyle();

	//Containers
		protected: Window* window;
		protected: Container* parent;

	//Identification
		private: std::string tag;
		private: std::string name;
		protected: std::vector<std::string> classes;

	//Size
		private: int minWidth, minHeight;
		private: int widthPercent, heightPercent;
		private: int widthOffset, heightOffset;

	//Location
		private: int xPercent, yPercent;
		private: int xOffset, yOffset;
		private: int z;
		private: float scrollOffsetX, scrollOffsetY;

	//Cached calculations
		private: int x, y;
		private: int width, height;
		private: int screenX, screenY;

	//Graphics
		protected: Graphics* graphics;
		private: bool graphicsUpdate;
		private: bool profileUpdate;//This was added to try to take out the setProfile call when updating graphics. @see oui::Container::redrawChildren()

	//State
		private: bool interactable;
		private: bool hovered, mouseDown;
		//The local mouse X position
		protected: int mouseX;
		//The local mouse Y position
		protected: int mouseY;
		private: bool visible;
		private: bool selected;
		//private: bool selectable;
		private: std::vector<std::u16string> rightClickOptions;

	//Styling
		private: int cursor;
		private: Color backgroundColor1;
		private: Color backgroundColor2;
		private: char borderStyle;
		private: int borderWidth;
		private: Color borderColor;
		private: bool centeredX;
		private: bool centeredY;
		private: int opacity;

		private: std::u16string currentProfileName;
        private: AttributeProfile* currentProfile;
		protected: Style* definedStyle;
		protected: Style* style;

		protected: std::map<char, std::vector<std::function<void(MouseEvent, Component*)>>> mouseEventHandlers;
		protected: std::map<char, std::vector<std::function<void(KeyEvent, Component*)>>> keyEventHandlers;
		protected: std::map<char, std::vector<std::function<void(WindowEvent, Component*)>>> windowEventHandlers;
		protected: std::vector<std::function<void(ScrollEvent, Component*)>> scrollEventHandlers;
		protected: std::vector<std::function<void(MenuEvent, Component*)>> menuEventHandlers;

/* END OF VARIABLES */

/* START OF METHODS */

	//Constructor/Destructor
		public: ~Component();

		//Default attributes are not loaded until the component is added, so do not attempt to get attribute values in the constructor
		public: Component(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing=false);

        public: void setAttribute(const std::string& name, Attribute val, const std::u16string& profile=u"default");
        public: Attribute getAttribute(const std::string& name, Attribute defaultVal=0);

	//Processing and Drawing
		public: virtual int process();
		public: virtual void redraw();
		public: void drawBorder(); 
		public: virtual void addedToContainer(Container* container);

	//Events
		public: virtual void handleEvent(Event&);

		public: void addEventListener(char type, std::function<void(MouseEvent, Component*)> handler);
		public: void addEventListener(char type, std::function<void(KeyEvent, Component*)> handler);
		public: void addEventListener(char type, std::function<void(WindowEvent, Component*)> handler);
		public: void addEventListener(char type, std::function<void(ScrollEvent, Component*)> handler);
		public: void addEventListener(char type, std::function<void(MenuEvent, Component*)> handler);
		//TODO removeEventListener
		
	//Usage functions
		//This parses a value as if it were an OSAL attribute
		//This method is much slower than setAttribute, but handles multiple parameters in the value. E.g: "parseAttribute("bg-color", u"0 0 0 255")"
		public: void parseAttribute(const std::string& name, const std::u16string& value, const std::u16string& profile = u"default");
		public: virtual void setProfile(const std::u16string& profile);
		public: bool contains(int localX, int localY);

	//Containers
		public: virtual bool isWindow();
		public: virtual bool isContainer();
		public: virtual Window* getWindow();
		public: virtual Container* getParent();
		//Recursively checks parents for the specified component
		public: bool isChildOf(Component* c);
		//Recursively looks for a parent with the specified tag
		public: bool isChildOf(const std::string& tag);

	//Identification
		public: std::string getTag();
		public: bool compareTag(const std::string& tag);
		public: virtual bool setName(const std::string& name);
		public: std::string getName();
		public: void addClass(std::string _class);
		public: bool removeClass(const std::string& _class);
		public: void setClasses(const std::string& classes);
		public: void setClasses(std::vector<std::string> classes);
		public: std::vector<std::string> getClasses();

	//Size & Location
		public: bool isDependantOnParent();
		public: int getWidth();
		public: int getHeight();
		public: int calculateWidth();
		public: int calculateHeight();
		public: int calculateX();
		public: int calculateY();
		public: int calculateScreenX();
		public: int calculateScreenY();
		public: int getX();
		public: int getY();
		public: int getScreenX();
		public: int getScreenY();
		public: void setScrollOffsetX(int scrollOffsetX);
		public: void setScrollOffsetY(int scrollOffsetY);

	//Graphics
		public: Graphics* getGraphics();
		public: void flagGraphicsUpdate(bool flagParent = true);
		public: bool needsGraphicsUpdate();
		public: void flagProfileUpdate();
		public: bool needsProfileUpdate();
		public: void clearProfileUpdate();
		
	//State
		public: int getMouseX();
		public: int getMouseY();
		//Sets this as the active component in the window
		public: virtual void setSelected(bool selected);//TODO make this change the window as well, and make it so comps are deselected when added to window
		public: bool isSelected();
		//This is used to distinguish a click from a release
		public: virtual void setMouseDown(bool mouseDown);
		public: bool isMouseDown();
		public: virtual void setHovered(bool hovered);
		public: virtual bool isHovered();

    
		
	//Styling
		//Creates a new StyleSheet via `Component::getAllStyleSheets()` and applies them to this component
		public: virtual Style* createStyle(StyleSheet* sheet = NULL);
		public: int getCursor();//TODO replace with Attribute value substitution (auto change string value to int value when attribute is added)
		public: virtual std::vector<std::u16string> getRightClickOptions();
		public: AttributeProfile* getCurrentProfile();
		public: virtual void updateStyle();
		public: virtual Style* getDefaultStyle();

/* END OF METHODS */
		
	};

}

#endif
