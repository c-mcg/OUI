#ifndef OUI_COMPONENT_H
#define OUI_COMPONENT_H

#include "OUI_Export.h"
#include "gfx/OUI_Graphics.h"
#include <vector>
#include <functional>
#include "attribute/OUI_StyleSheet.h"

#include "event/OUI_EventDispatcher.h"

namespace oui {

    class Window;
    class Container;

    struct Bounds {
        int top, bottom;
        int left, right;
    };

    class Component {

        public:

            /**
            * @brief Adds a listener to the specified event
             * 
             * @param type The type of event to listen for
             * @param handler The function to be run when the event occurs
             */
            void addEventListener(std::string type, EventHandler handler);


		protected:
			
            /**
             * @brief Handles event listeners and dispatching for the component
             */
			EventDispatcher* eventDispatcher;


/* START OF VARIABLES */

	public:

		const bool needsProcessing;

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

/* END OF VARIABLES */

/* START OF METHODS */

    //Constructor/Destructor
        public: OUI_API ~Component();

        //Default attributes are not loaded until the component is added, so do not attempt to get attribute values in the constructor
        public: OUI_API Component(const std::string& tag, const std::string& name, const std::string& classes, bool needsProcessing=false, EventDispatcher* eventDispatcher=new EventDispatcher());

        public: void setAttribute(const std::string& name, Attribute val, const std::u16string& profile=u"default");
        public: OUI_API Attribute getAttribute(const std::string& name, Attribute defaultVal=0);

    //Processing and Drawing
        public: OUI_API virtual int process();
        public: OUI_API virtual void redraw();
        public: OUI_API void drawBorder();
        public: OUI_API virtual void addedToContainer(Container* container);

        public: EventDispatcher* getEventDispatcher();
        
    //Usage functions
        //This parses a value as if it were an OSAL attribute
        //This method is much slower than setAttribute, but handles multiple parameters in the value. E.g: "parseAttribute("bg-color", u"0 0 0 255")"
        public: OUI_API void parseAttribute(const std::string& name, const std::u16string& value, const std::u16string& profile = u"default");
        public: OUI_API virtual void setProfile(const std::u16string& profile);
        public: OUI_API bool contains(int localX, int localY);

    //Containers
        public: OUI_API virtual bool isWindow();
        public: OUI_API virtual bool isContainer();
        public: OUI_API virtual Window* getWindow();
        public: OUI_API virtual Container* getParent();
        //Recursively checks parents for the specified component
        public: OUI_API bool isChildOf(Component* c);
        //Recursively looks for a parent with the specified tag
        public: OUI_API bool isChildOf(const std::string& tag);

    //Identification
        public: OUI_API std::string getTag();
        public: OUI_API bool compareTag(const std::string& tag);
        public: OUI_API virtual bool setName(const std::string& name);
        public: OUI_API std::string getName();
        public: OUI_API void addClass(std::string _class);
        public: OUI_API bool removeClass(const std::string& _class);
        public: OUI_API void setClasses(const std::string& classes);
        public: OUI_API void setClasses(std::vector<std::string> classes);
        public: OUI_API std::vector<std::string> getClasses();

    //Size & Location
        public: OUI_API bool isDependantOnParent();
        public: OUI_API int getWidth();
        public: OUI_API int getHeight();
        public: OUI_API int calculateWidth();
        public: OUI_API int calculateHeight();
        public: OUI_API int calculateX();
        public: OUI_API int calculateY();
        public: OUI_API int calculateScreenX();
        public: OUI_API int calculateScreenY();
        public: OUI_API int getX();
        public: OUI_API int getY();
        public: OUI_API int getScreenX();
        public: OUI_API int getScreenY();
        public: OUI_API void setScrollOffsetX(int scrollOffsetX);
        public: OUI_API void setScrollOffsetY(int scrollOffsetY);

    //Graphics
        public: OUI_API Graphics* getGraphics();
        public: OUI_API void flagGraphicsUpdate(bool flagParent = true);
        public: OUI_API bool needsGraphicsUpdate();
        public: OUI_API void flagProfileUpdate();
        public: OUI_API bool needsProfileUpdate();
        public: OUI_API void clearProfileUpdate();
        
    //State
        public: OUI_API int getMouseX();
        public: OUI_API int getMouseY();
        //Sets this as the active component in the window
        public: OUI_API virtual void setSelected(bool selected);//TODO it's vague wether this is callable from a runtime
        public: OUI_API bool isSelected();
        //This is used to distinguish a click from a release
        public: OUI_API virtual void setMouseDown(bool mouseDown);
        public: OUI_API bool isMouseDown();
        public: OUI_API virtual void setHovered(bool hovered);
        public: OUI_API virtual bool isHovered();

    //Styling
        //Creates a new StyleSheet via `Component::getAllStyleSheets()` and applies them to this component
        public: OUI_API virtual Style* createStyle(StyleSheet* sheet = NULL);
        public: OUI_API int getCursor();
        public: OUI_API virtual std::vector<std::u16string> getRightClickOptions();
        public: OUI_API AttributeProfile* getCurrentProfile();
        public: OUI_API virtual void updateStyle();
        public: OUI_API virtual Style* getDefaultStyle();

/* END OF METHODS */
    };

}

#endif
