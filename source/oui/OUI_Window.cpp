
#include "oui/OUI_Window.h"
#include <SDL.h>
#include "OUI.h"
#include <iostream>
#include <math.h>
#include "OUI_KeyCodes.h"
#include "OUI_Constants.h"
#include "oui/comp/OUI_Menu.h"
#include "oui/comp/OUI_ScrollPanel.h"
#include "oui/xplat/OUI_CrossPlatform.h"
#include "util/OUI_StringUtil.h"


#ifdef _DEBUG
	#include "OUI_Context.h"
#endif

oui::Window::~Window() {
	SDL_DestroyWindow(baseWindow);
	SDL_DestroyRenderer(renderer);
	std::vector<EditEvent*>::iterator it;
	for (it = editEvents.begin(); it != editEvents.end();) {
		delete *it;
		it = editEvents.erase(it);
	}
	selectedComponent = NULL;
	rightClickedComponent = NULL;
}

oui::Window::Window() : Container("window", "window", "window") {//TODO let you set a name
	window = this;
	int screenWidth = 1366;//TODO;
	int screenHeight = 768;//TODO

	this->context = NULL;

	AttributeProfile* profile = style->getOrCreateProfile(u"default");//This should definitely be default at this point

	//this->widthOffset = screenWidth / 2;
	//this->heightOffset = screenHeight / 2;
	int width = screenWidth / 2;
	int height = screenHeight / 2;
	setAttribute("width-offset", width);
	setAttribute("height-offset", height);
	//this->xPercent = widthOffset - widthOffset / 2;
	//this->yPercent = heightOffset - heightOffset / 2;
	int widthOffset = profile->getInt("width-offset");
	int heightOffset = profile->getInt("height-offset");
	this->title = u"";
	this->visible = false;
	this->selectedComponent = NULL;
	this->cursor = -1;

	//setProfile(u"default");

	this->graphics = new oui::Graphics(getWidth(), getHeight());
	this->baseWindow = SDL_CreateWindow(convertUTF16toUTF8(getTitle()).c_str(), 0, 0, widthOffset, heightOffset, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(baseWindow, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cout << "NULL RENDERER" << std::endl;
		//TODO error
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	getGraphics()->setRenderer(renderer);
	this->baseWindowID = SDL_GetWindowID(baseWindow);
	std::cout << "name=" << getName().c_str() << " id=" << baseWindowID << std::endl;
	

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



void oui::Window::setContext(oui::Context* context) {
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
			if (newVisible) {
				//Not sure if this commented code will ever be needed
				//It updates the location and size when a window is shown
				//parseAttribute("location", std::u16string("0 0 ") + intToString(w->globalMouseX - w->moveOffX) + " " + intToString(w->globalMouseY - w->moveOffY));
				//setLocation(getProfile("default")->getInt("x-percent"), getProfile("default")->getInt("y-percent"), getProfile("default")->getInt("x-offset"), getProfile("default")->getInt("y-offset"));
				//setSize(getWidth(), getHeight());
				std::cout << "base window showing..." << std::endl;
				SDL_ShowWindow(baseWindow);
				std::cout << "base window shown" << std::endl;
			} else {
				SDL_HideWindow(baseWindow);
			}
			this->visible = newVisible;
		}

	}
	
	if (w != getWidth() || h != getHeight()) {
		SDL_SetWindowSize(baseWindow, getWidth(), getHeight());
		flagGraphicsUpdateAll();
	}

	if (x != getX() || y != getY()) {
		SDL_SetWindowPosition(baseWindow, getX(), getY());
	}

}

bool oui::Window::isWindow() {
	return true;
}

int oui::Window::process() {
	if (quit) {
		return -1;
	}

	CrossPlatform::getGlobalMousePos(this, globalMouseX, globalMouseY);

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

	//Container::process();

	for (int i = 0; i < processableChildren.size(); i++) {
		auto child = processableChildren.at(i);
		if (child->needsProcessing) {
			child->process();
		}
	}

	if (needsProfileUpdate()) {
		updateStyle();
		clearProfileUpdate();
	}

	if (window->needsGraphicsUpdate()) {
		//TODO needsGraphicsUpdate is called because of DrawablePanel so profile doesn't need to change...
		window->updateStyle();
		window->redraw();
		SDL_RenderPresent(renderer);
	}

	return 0;
}

void oui::Window::handleSDLEvent(SDL_Event* event) {
	if (event->type == SDL_WINDOWEVENT && event->window.windowID == baseWindowID) {
		switch (event->window.event) {
			case SDL_WINDOWEVENT_SHOWN:
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				break;
			case SDL_WINDOWEVENT_MOVED:
				break;
			case SDL_WINDOWEVENT_RESIZED:
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				break;
			case SDL_WINDOWEVENT_RESTORED:
				break;
			case SDL_WINDOWEVENT_ENTER:
				cursor = -1;
				break;
			case SDL_WINDOWEVENT_LEAVE:
				setHovered(false);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				focused = true;
				flagGraphicsUpdateAll();
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				focused = false;
				flagGraphicsUpdateAll();
				break;
			case SDL_WINDOWEVENT_CLOSE:
				//TOOO should call event
				close();
				break;
				#if SDL_VERSION_ATLEAST(2, 0, 5)
			case SDL_WINDOWEVENT_TAKE_FOCUS:
				//TODO modal windows
				break;
			case SDL_WINDOWEVENT_HIT_TEST:
				break;
				#endif
			default:
				SDL_Log("Window %d got unknown event %d",
						baseWindowID, event->window.event);
				break;
		}
	}

	if (event->type == SDL_MOUSEBUTTONUP && event->button.windowID == baseWindowID) {
		moving = false;
		resizing = false;

		mouseX = event->button.x;
		mouseY = event->button.y;
		Component* c = getComponentAt(mouseX, mouseY);
		if (c->isMouseDown()) {
			MouseEvent e = MouseEvent(Event::CLICKED, mouseX - c->getScreenX(), mouseY - c->getScreenY());
			c->handleEvent(e);
		}
		setMouseDown(false);
		MouseEvent e = MouseEvent(Event::MOUSE_UP, mouseX - c->getScreenX(), mouseY - c->getScreenY());
		c->handleEvent(e);
	}

	if (event->type == SDL_MOUSEBUTTONDOWN && (event->button.windowID == baseWindowID || focused)) {
		mouseX = event->button.x;
		mouseY = event->button.y;

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
			if (event->button.button == 1) {//Left Click
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
				MouseEvent e = MouseEvent(Event::MOUSE_DOWN, mouseX - c->getScreenX(), mouseY - c->getScreenY());
				c->handleEvent(e);
			} else if (event->button.button == 2) {//Right click
				std::cout << "mb2" << std::endl;
			} else if (event->button.button == 3) {
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

	if (event->type == SDL_MOUSEMOTION && (event->motion.windowID == baseWindowID || focused)) {
		mouseX = event->motion.x;
		mouseY = event->motion.y;
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
		MouseEvent e = MouseEvent(Event::MOUSE_MOVE, mouseX - c->getScreenX(), mouseY - c->getScreenY());

		c->handleEvent(e);
	}

	if (event->type == SDL_MOUSEWHEEL && (event->wheel.windowID == baseWindowID || focused)) {
		ScrollEvent e = ScrollEvent(event->wheel.y);

		Component* c = getComponentAt(mouseX, mouseY);

		c->handleEvent(e);
		if (!c->compareTag("scrollpanel") && !c->compareTag("scrollbar")) {
			Component* parent = c->getParent();
			while (parent != NULL) {
				if (parent->compareTag("scrollpanel") || parent->compareTag("scrollbar")) {
					parent->handleEvent(e);
					break;
				}
				parent = parent->getParent();
			}
		}
	}

	if (event->type == SDL_KEYDOWN && (event->key.windowID == baseWindowID || focused)) {
		int code = event->key.keysym.sym;
		if (code == KEY_CONTROL_LEFT || code == KEY_CONTROL_RIGHT) {
			ctrlDown = true;
		}
		if (code == KEY_SHIFT_LEFT || code == KEY_SHIFT_RIGHT) {
			shiftDown = true;
		}
		if (code == KEY_ALT_LEFT || code == KEY_ALT_RIGHT) {
			altDown = true;
		}

		if (code == KEY_Z && ctrlDown) {
			undo();
		} else if (code == KEY_Y && ctrlDown) {
			redo();
		} else {
			Component* c = getSelectedComponent();
			if (c != NULL) {
				KeyEvent e = KeyEvent(Event::KEY_TYPED, code, getChar(code, isShiftDown()));

				c->handleEvent(e);

				Container* parent = c->getParent();
				while (parent != NULL) {
					parent->handleEvent(e);
					parent = parent->getParent();
				}
			}
		}
	}
	if (event->type == SDL_KEYUP && (event->key.windowID == baseWindowID || focused)) {
		int code = event->key.keysym.sym;
		if (code == KEY_CONTROL_LEFT || code == KEY_CONTROL_RIGHT) {
			ctrlDown = false;
		}
		if (code == KEY_SHIFT_LEFT || code == KEY_SHIFT_RIGHT) {
			shiftDown = false;
		}
		if (code == KEY_ALT_LEFT || code == KEY_ALT_RIGHT) {
			altDown = false;
		}

		Component* c = getSelectedComponent();
		if (c != NULL) {
			KeyEvent e = KeyEvent(Event::KEY_UP, code, getChar(code, isShiftDown()));

			c->handleEvent(e);

			Container* parent = c->getParent();
			while (parent != NULL) {
				parent->handleEvent(e);
				parent = parent->getParent();
			}
		}
	}
}

oui::Graphics* oui::Window::getGraphics() {
	return graphics;
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
		oui::Graphics* g = graphics;

		Container::redraw();
		SDL_SetRenderTarget(g->getRenderer(), NULL);
		g->render(0, 0);
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
	SDL_SetWindowTitle(baseWindow, convertUTF16toUTF8(title).c_str());

	if (getChild("windowBar") != NULL) {
		Label* titleLbl = ((Label*) ((Container*) getChild("windowBar"))->getChild("titleLbl"));
		if (titleLbl != NULL) {
			titleLbl->setAttribute("text", title);
		}
	}
}

void oui::Window::setCursor(int cursor) {
	if (this->cursor == cursor) {
		return;
	}
	this->cursor = cursor;
	SDL_SystemCursor newCursor;
	switch (cursor) {
		case CURSOR_DEFAULT:
			newCursor = SDL_SYSTEM_CURSOR_ARROW;
			break;
		case CURSOR_POINTER:
			newCursor = SDL_SYSTEM_CURSOR_HAND;
			break;
		case CURSOR_RESIZE:
			newCursor = SDL_SYSTEM_CURSOR_SIZENWSE;
			break;
		case CURSOR_TEXT:
			newCursor = SDL_SYSTEM_CURSOR_IBEAM;
			break;
	}

	if (this->baseCursor != NULL) {
		//	SDL_FreeCursor(baseCursor);
	}
	this->baseCursor = SDL_CreateSystemCursor(newCursor);
	SDL_SetCursor(baseCursor);

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
	this->quit = true;
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

		CrossPlatform::getMaximizeSize(this, maximizedX, maximizedY, maximizedWidth, maximizedHeight);

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

	Style* defaultStyle = getDefaultStyle();

	AttributeProfile* defaultProfile = style->getOrCreateProfile(u"default");
	AttributeProfile* hoverProfile = style->getOrCreateProfile(u"hover");
	
	defaultProfile->clearDefaultProfiles();
	defaultProfile->addDefaultProfile(defaultStyle->getOrCreateProfile(u"default"));

	hoverProfile->clearDefaultProfiles();
	hoverProfile->addDefaultProfile(defaultStyle->getOrCreateProfile(u"hover"));
	hoverProfile->addDefaultProfile(defaultProfile);

	setProfile(u"default");

	setAttribute("visible", true);
}

void oui::Window::minimize() {
  Event e = Event(Event::WINDOW_MINIMIZED);
	handleEvent(e);
	SDL_MinimizeWindow(baseWindow);
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