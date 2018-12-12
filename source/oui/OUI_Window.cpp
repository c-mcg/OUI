#include "os/OUI_OS.h"
#include "oui/OUI_Window.h"
#include <SDL.h>
#include "OUI.h"
#include <iostream>
#include <math.h>
#include "OUI_Constants.h"
#include "oui/comp/OUI_Menu.h"
#include "oui/comp/OUI_ScrollPanel.h"
#include "util/OUI_StringUtil.h"
#include "os/OUI_OS.h"


#ifdef _DEBUG
	#include "OUI_Context.h"
#endif

oui::Window::~Window() {
	std::vector<EditEvent*>::iterator it;
	for (it = editEvents.begin(); it != editEvents.end();) {
		delete *it;
		it = editEvents.erase(it);
	}
	selectedComponent = NULL;
	rightClickedComponent = NULL;
}

oui::Window::Window(int width, int height) : Container("window", "window", "window") {//TODO let you set a name
}

void oui::Window::initializeWindow(int width, int height) {
    
    graphics = createGraphics(width, height);
    graphics->setFont(Font::getFont(u"notoserif", 12, this));
    window = this;

	this->context = NULL;

	AttributeProfile* profile = style->getOrCreateProfile(u"default");//This should definitely be default at this point

	width = width / 2;
	height = height / 2;
	setAttribute("width-offset", width);
	setAttribute("height-offset", height);
	int widthOffset = profile->getInt("width-offset");
	int heightOffset = profile->getInt("height-offset");
	this->title = u"";
	this->visible = false;
	this->selectedComponent = NULL;
	this->rightClickedComponent = NULL;
	this->cursor = -1;
	this->shutdown = false;
	this->focused = false;
	this->cursorType = CURSOR_DEFAULT;
	this->editIndex = 0;

	this->maximized = false;
	this->moving = false;
	this->resizing = false;

	this->shiftDown = false;
	this->ctrlDown = false;
	this->altDown = false;

	Panel* windowBar = new Panel("windowBar", "");
	windowBar->setAttribute("cursor", u"pointer");
	windowBar->setAttribute("cursor", u"pointer");
	windowBar->parseAttribute("size", u"100 0 0 26");
	windowBar->setAttribute("z-index", 10);
	windowBar->parseAttribute("bg-color", u"208 208 208 255");
	windowBar->parseAttribute("bg-color", u"0 0 0 255");

	windowBar->addEventListener(Event::MOUSE_DOWN, [this](MouseEvent e, Component* c) {
		if (maximized) {
			maximize();
			moveOffX = c->getWidth() / 2;
			moveOffY = c->getHeight() / 2;
			parseAttribute("location", u"0 0 " + intToString(globalMouseX - moveOffX) + u" " + intToString(globalMouseY - moveOffY));
		} else {
			moveOffX = getMouseX();
			moveOffY = getMouseY();
		}
		moving = true;
	});

	windowBar->addEventListener(Event::MOUSE_UP, [this](MouseEvent e, Component* c) {
		moving = false;
		resizing = false;
	});

	Label* titleLbl = new Label("titleLbl", "windowText");
	titleLbl->parseAttribute("location", u"0 50 4 0");
	titleLbl->parseAttribute("font", u"notoserif 14");
	titleLbl->setAttribute("centered-y", true);
	titleLbl->setAttribute("auto-size", true);
	titleLbl->setAttribute("interactable", false);

	Button* minimizeBtn = new Button("minimizeBtn", "windowButton");

	minimizeBtn->addEventListener(Event::CLICKED, [this](MouseEvent e, Component* c) {
		minimize();
	});
	minimizeBtn->parseAttribute("location", u"100 0 -73 0");
	minimizeBtn->parseAttribute("size", u"0 0 25 25");
	minimizeBtn->parseAttribute("bg-color", u"200 200 200 255");
	minimizeBtn->setAttribute("border-style", u"solid");
	minimizeBtn->setAttribute("image", u"minimize-btn.png");

	Button* maximizeBtn = new Button("maximizeBtn", "windowButton");

	maximizeBtn->addEventListener(Event::CLICKED, [this](MouseEvent e, Component* c) {
		maximize();
	});
	maximizeBtn->parseAttribute("location", u"100 0 -49 0");
	maximizeBtn->parseAttribute("size", u"0 0 25 25");
	maximizeBtn->parseAttribute("bg-color", u"200 200 200 255");
	maximizeBtn->setAttribute("border-style", u"solid");
	maximizeBtn->setAttribute("image", u"maximize-btn1.png");

	Button* closeBtn = new Button("closeBtn", "windowButton");
	closeBtn->addEventListener(Event::CLICKED, [this](MouseEvent e, Component* c) {
		WindowEvent windowEvt = WindowEvent(Event::WINDOW_CLOSED);
		handleEvent(windowEvt);
	});

	closeBtn->parseAttribute("location", u"100 0 -25 0");
	closeBtn->parseAttribute("size", u"0 0 25 25");
	closeBtn->parseAttribute("bg-color", u"200 200 200 255");
	closeBtn->setAttribute("border-style", u"solid");
	closeBtn->setAttribute("image", u"close-btn.png");

	windowBar->addChild(titleLbl);
	windowBar->addChild(minimizeBtn);
	windowBar->addChild(maximizeBtn);
	windowBar->addChild(closeBtn);
	addChild(windowBar);

	Menu* menu = new Menu("rightClickMenu", "rightClickMenu");
	menu->parseAttribute("size", u"0 0 200 100");
	menu->setAttribute("visible", false);
	menu->setAttribute("z-index", 15);

	menu->addEventListener(Event::CLICKED, [this](MouseEvent e, Component* c) {
		getRightClickedComponent()->handleEvent(e);
		c->setAttribute("visible", false);
		setSelectedComponent(getRightClickedComponent());
	});
	addChild(menu);

	parseAttribute("bg-color", u"255 255 255 255");
	setAttribute("border-style", u"solid");
}

void oui::Window::setContext(Context* context) {
	this->context = context;
}

oui::Context* oui::Window::getContext() {
	return this->context;
}

void oui::Window::setProfile(std::u16string profileName) {
	bool wasVisible = this->visible;
	int w = getWidth(), h = getHeight(), x = getX(), y = getY();

	Container::setProfile(profileName);

	AttributeProfile* profile = style->getProfile(profileName);
	if (profile != NULL) {

		bool newVisible = profile->getBool("visible");
		if (wasVisible != newVisible) {
            //setVisible(newVisible);
            if (newVisible) {
                showWindow();
            } else {
                hideWindow();
            }
			this->visible = newVisible;
		}

	}
	
	if (w != getWidth() || h != getHeight()) {
        setSize(getWidth(), getHeight());
		flagGraphicsUpdateAll();
	}

	if (x != getX() || y != getY()) {
		setPosition(getX(), getY());
	}

}

bool oui::Window::isWindow() {
	return true;
}

int oui::Window::process() {
	if (shutdown) {
		std::cout << "Shutdown was true.." << std::endl;
		return -1;
	}

	OS()->getGlobalMousePos(this, globalMouseX, globalMouseY);

	if (moving) {
		int newX = globalMouseX - moveOffX;
		int newY = globalMouseY - moveOffY;
		if (newX != getCurrentProfile()->getInt("x-offset") || newY != getCurrentProfile()->getInt("y-offset")) {
			this->setAttribute("x-offset", newX);
			this->setAttribute("y-offset", newY);
		}
	}

	if (resizing) {
		this->parseAttribute("size", u"0 0 " + intToString(globalMouseX - getX() + resizeX) + u" " + intToString(globalMouseY - getY() + resizeY));
	}


	auto size = queuedEvents.size();
    for (auto i = 0; i < size; ++i)
    {
        if (currentTimeMillis() - queuedEvents[i]->queueTime >= queuedEvents[i]->delay) {
             queuedEvents[i]->func();
			 delete queuedEvents[i];
			 queuedEvents.erase(queuedEvents.begin() + i);
			 i--;
			 size--;
        }
    }

	if (needsProfileUpdate()) {
		updateStyle();
		clearProfileUpdate();
	}

	Container::process();

	for (int i = 0; i < processableChildren.size(); i++) {
		auto child = processableChildren.at(i);
		if (child->needsProcessing) {
			child->process();
		}
	}

	if (needsGraphicsUpdate()) {
		//TODO needsGraphicsUpdate is called because of DrawablePanel so profile doesn't need to change...
		updateStyle();
		redraw();
        render();
	}

	return 0;
}

oui::Graphics* oui::Window::getGraphics() {
	return graphics;
}

void oui::Window::handleMouseMoveEvent(MouseEvent event) {
    mouseX = event.getX();
    mouseY = event.getY();

    Component* c = getComponentAt(mouseX, mouseY);
    //std::cout << "Component: name=" << getName() << " mX=" << mouseX << " mY=" << mouseY << std::endl;
    if (c->getParent() != NULL) {
        Container* parent = (Container*) c->getParent();
        for (unsigned int i = 0; i < getNumChildren(); i++) {
            Component* comp = getChild(i);
            if (comp != NULL) {
                if (comp != c && !c->isChildOf(comp)) {
                    comp->setHovered(false);
                }
            }
        }
        for (unsigned int i = 0; i < parent->getNumChildren(); i++) {
            Component* comp = parent->getChild(i);
            if (comp != NULL) {
                comp->setHovered(false);
            }
        }
        if (c->isContainer()) {
            Container* cont = (Container*) c;
            for (unsigned int i = 0; i < cont->getNumChildren(); i++) {
                Component* comp = cont->getChild(i);
                if (comp != NULL) {
                    comp->setHovered(false);
                }
            }
        }
    }
    if (!resizing) {
        int dist = (int) abs(sqrt(pow(globalMouseX - getX() - getWidth(), 2) + pow(globalMouseY - getY() - getHeight(), 2)));
        if (dist < 15) {
            setCursor(CURSOR_RESIZE);
        } else {
            setCursor(c->getCursor());
        }
    }
    c->setHovered(true);
    MouseEvent e = MouseEvent(Event::MOUSE_MOVE, mouseX - c->getScreenX(), mouseY - c->getScreenY(), mouseX, mouseY);
    c->handleEvent(e);
}

void oui::Window::handleMouseDownEvent(MouseEvent event) {
    mouseX = event.getX();
    mouseY = event.getY();

    int dist = (int) abs(sqrt(pow(globalMouseX - getX() - getWidth(), 2) + pow(globalMouseY - getY() - getHeight(), 2)));
    if (dist < 15) {
        Component* menu = getChild("rightClickMenu");
        if (menu != NULL && menu->getCurrentProfile()->getBool("visible")) {
            menu->setAttribute("visible", false);
        }
        resizeX = getWidth() - (globalMouseX - getX());
        resizeY = getHeight() - (globalMouseY - getY());
        resizing = true;
    } else {
        Component* c = getComponentAt(mouseX, mouseY);
        Component* menu = getChild("rightClickMenu");
        if (menu != NULL && menu->getCurrentProfile()->getBool("visible") && c != menu && !c->isChildOf(menu)) {
            menu->setAttribute("visible", false);
        }
        if (event.getButton() == 1) {//Left Click
            for (unsigned int i = 0; i < getNumChildren(); i++) {
                Component* comp = getChild(i);
                if (comp != NULL) {
                    if (comp != c && !c->isChildOf(comp)) {
                        comp->setMouseDown(false);
                    }
                }
            }
            if (c->isContainer()) {
                Container* cont = (Container*) c;
                for (unsigned int i = 0; i < cont->getNumChildren(); i++) {
                    Component* comp = cont->getChild(i);
                    if (comp != NULL) {
                        comp->setMouseDown(false);
                    }
                }
            }
            if (c->getParent() != NULL) {
                Container* parent = (Container*) c->getParent();
                for (unsigned int i = 0; i < parent->getNumChildren(); i++) {
                    Component* comp = parent->getChild(i);
                    if (comp != NULL && comp != c) {
                        comp->setMouseDown(false);
                    }
                }
            }
            c->setMouseDown(true);
            setSelectedComponent(c);
            MouseEvent e = MouseEvent(Event::MOUSE_DOWN, mouseX - c->getScreenX(), mouseY - c->getScreenY(), mouseX, mouseY);
            c->handleEvent(e);
        } else if (event.getButton() == 2) {//Right click
            std::cout << "mb2" << std::endl;
        } else if (event.getButton() == 3) {
            Menu* menu = (Menu*) getChild("rightClickMenu");
            std::vector<std::u16string> rcOptions = c->getRightClickOptions();
            if (menu != NULL && rcOptions.size() != 0) {
                setSelectedComponent(c);//TODO pass right click to object
                setRightClickedComponent(c);

                std::u16string arguments = u"";
                for (int i = 0; i < rcOptions.size(); i++) {
                    arguments += u"\"" + rcOptions.at(i) + u"\"" + (i < rcOptions.size() - 1 ? u" " : u"");
                }

#ifdef _DEBUG

                arguments += u" Inspect";

#endif

                menu->parseAttribute("options", arguments);

                int x = mouseX;
                int y = mouseY;
                if (mouseX + menu->getWidth() > getWidth()) {
                    x = getWidth() - menu->getWidth();
                }
                if (y + menu->getHeight() > getHeight()) {
                    y -= menu->getHeight();
                }
                menu->parseAttribute("location", u"0 0 " + intToString(x) + u" " + intToString(y));
                menu->setAttribute("visible", true);
                std::cout << "menuX=" << menu->getX() << " menuY=" << menu->getY() << " mW=" << menu->getWidth() << " mH=" << menu->getHeight() << std::endl;
            } else if (menu != NULL && menu->getCurrentProfile()->getBool("visible")) {
                menu->setAttribute("visible", false);
            }
        }
    }
}

void oui::Window::handleMouseUpEvent(MouseEvent event) {
    moving = false;
    resizing = false;

    mouseX = event.getX();
    mouseY = event.getY();
    Component* c = getComponentAt(mouseX, mouseY);
    if (c->isMouseDown()) {
        MouseEvent e = MouseEvent(Event::CLICKED, mouseX - c->getScreenX(), mouseY - c->getScreenY(), mouseX, mouseY);
        c->handleEvent(e);
    }
    setMouseDown(false);
    MouseEvent e = MouseEvent(Event::MOUSE_UP, mouseX - c->getScreenX(), mouseY - c->getScreenY(), mouseX, mouseY);
    c->handleEvent(e);
}

void oui::Window::handleEvent(Event e) {

	Component::handleEvent(e);

	if (e.defaultPrevented) {
		return;
	}

	//eventHandler(e, c);
	if(e.type == Event::WINDOW_CLOSED) {//TODO shouldn't this be passed to eventHandler(e, c) ?
		this->close();
	}

	if (e.type == Event::MOUSE_DOWN) {

	}
	if (e.type == Event::MOUSE_UP) {

	}
}

void oui::Window::redraw() {
	if (getCurrentProfile()->getBool("visible")) {
		Graphics* g = graphics;

		Container::redraw();
		g->renderToWindow(0, 0);
	}
}

void oui::Window::setTimeout(int delay, std::function<void()> func) {
	QueuedEvent* evnt = new QueuedEvent();
	evnt->delay = delay;
	evnt->func = func;
	evnt->queueTime = currentTimeMillis();
	queuedEvents.push_back(evnt);
}

void oui::Window::setTitle(std::u16string title) {
	if (this->title == title) {
		return;
	}
	this->title = title;
	
	if (getChild("windowBar") != NULL) {
		Label* titleLbl = ((Label*) ((Container*) getChild("windowBar"))->getChild("titleLbl"));
		if (titleLbl != NULL) {
			titleLbl->setAttribute("text", title);
		}
	}
}

bool oui::Window::setCursor(int cursor) {
	if (this->cursor == cursor) {
		return false;
	}
	this->cursor = cursor;
    return true;
}

bool oui::Window::hasFocus() {
	return focused;
}

void oui::Window::addEditEvent(EditEvent* e, bool append) {
	if (editIndex != editEvents.size() - 1) {
		std::vector<EditEvent*>::iterator it;
		for (it = editEvents.begin() + editIndex; it != editEvents.end(); it++) {
			delete *it;
		}
		editEvents.erase(editEvents.begin() + editIndex, editEvents.end());
	}
	EditEvent* last = editEvents.size() > 0 ? editEvents.at(editEvents.size() - 1) : NULL;
	if (append && last != NULL && e->isAppendable() && last->isAppendable() && e->getSource() == last->getSource()) {
		last->combine(*e);
		delete e;
	} else {
		editEvents.push_back(e);
		editIndex = editEvents.size() - 1;
	}
}
void oui::Window::undo() {
	if (editIndex < editEvents.size() && (editIndex != 0 || editEvents.at(0)->hasPerformed())) {
		editEvents.at(editIndex)->performUndo();
		editIndex--;
		if (editIndex < 0) {
			editIndex = 0;
		}
	}
}
void oui::Window::redo() {
	if (editIndex >= 0 && (editIndex != editEvents.size() - 1 || !editEvents.at(editEvents.size() - 1)->hasPerformed())) {
		editEvents.at(editIndex)->performRedo();
		editIndex++;
		if (editIndex > editEvents.size() - 1) {
			editIndex = editEvents.size() - 1;
		}
	}
}

bool oui::Window::isShiftDown() {
	return shiftDown;
}

bool oui::Window::isCtrlDown() {
	return ctrlDown;
}

bool oui::Window::isAltDown() {
	return altDown;
}

void oui::Window::setSelectedComponent(Component* component) {
	if (selectedComponent != NULL && selectedComponent != component) {
		selectedComponent->setSelected(false);
	}
	if (component != NULL) {
		component->setSelected(true);
	}
	this->selectedComponent = component;
}
oui::Component* oui::Window::getSelectedComponent() {
	return selectedComponent;
}

void oui::Window::setRightClickedComponent(Component* component) {
	this->rightClickedComponent = component;
}
oui::Component* oui::Window::getRightClickedComponent() {
	return rightClickedComponent;
}

std::u16string oui::Window::getTitle() {
	return title;
}

void oui::Window::close() {
	std::cout << "Window.close was called" << std::endl;
	this->shutdown = true;
}

void oui::Window::maximize() {
	Button* maximizeBtn = ((Button*) ((Container*) getChild("windowBar"))->getChild("maximizeBtn"));
	if (maximizeBtn != NULL) {
		maximizeBtn->setAttribute("image", u"maximizeBtn" + convertUTF8toUTF16(maximized ? "1" : "2") + u".png");
	}
	if (!maximized) {
		maximizeX = getX();
		maximizeY = getY();
		maximizeWidth = getWidth();
		maximizeHeight = getHeight();

		//TODO: cross platform
		

		int maximizedX = 0;
		int maximizedY = 0;
		int maximizedWidth = 0;
		int maximizedHeight = 0;

		OS()->getMaximizeSize(this, maximizedX, maximizedY, maximizedWidth, maximizedHeight);

		parseAttribute("location", u"0 0 " + intToString(maximizedX) + u" " + intToString(maximizedY));
		parseAttribute("size", u"0 0 " + intToString(maximizedWidth) + u" " + intToString(maximizedHeight));
		maximized = true;
		

	} else {
		maximized = false;

		parseAttribute("location", u"0 0 " + intToString(maximizeX) + u" " + intToString(maximizeY));
		parseAttribute("size", u"0 0 " + intToString(maximizeWidth) + u" " + intToString(maximizeHeight));
	}
	updateStyle();
}

void oui::Window::setVisible(bool visible) {

    this->visible = visible;

	Style* defaultStyle = getDefaultStyle();

	AttributeProfile* defaultProfile = style->getOrCreateProfile(u"default");
	AttributeProfile* hoverProfile = style->getOrCreateProfile(u"hover");
	
	defaultProfile->clearDefaultProfiles();
	defaultProfile->addDefaultProfile(defaultStyle->getOrCreateProfile(u"default"));

	hoverProfile->clearDefaultProfiles();
	hoverProfile->addDefaultProfile(defaultStyle->getOrCreateProfile(u"hover"));
	hoverProfile->addDefaultProfile(defaultProfile);

    if (visible) {
	    setProfile(u"default");
    }

	setAttribute("visible", true);
}

void oui::Window::minimize() {
    Event e = Event(Event::WINDOW_MINIMIZED);
    handleEvent(e);
}

void oui::Window::setClipboardText(std::u16string text) {
	SDL_SetClipboardText(convertUTF16toUTF8(text).c_str());//TODO clipboar should support wstring
}

std::u16string oui::Window::getClipboardText() {
	return convertUTF8toUTF16(SDL_GetClipboardText());
}
bool oui::Window::hasClipboardText() {
	return SDL_HasClipboardText();
}

oui::Window* oui::Window::getWindow() {
	return (Window*) parent;
}

oui::Graphics* oui::Window::createGraphics(int width, int height) {
    return NULL;
}


void oui::Window::render() {

}
void oui::Window::showWindow() {

}
void oui::Window::hideWindow() {

}
void oui::Window::setSize(int width, int height) {

}
void oui::Window::setPosition(int width, int height) {
}