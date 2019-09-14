#include "os/OUI_OS.h"
#include "OUI_Window.h"
#include <SDL.h>
#include "OUI.h"
#include <iostream>
#include <math.h>
#include "components/menu/OUI_Menu.h"
#include "components/scrollpanel/OUI_ScrollPanel.h"
#include "util/OUI_StringUtil.h"
#include "os/OUI_OS.h"
#include "components/OUI_ComponentLoader.h"

#include "event/OUI_WindowEvent.h"
#include "event/OUI_ScrollEvent.h"
#include "event/OUI_KeyboardEvent.h"

#include <functional>

#include "OUI_KeyCodes.h"

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
}

oui::Window::Window(int width, int height, EventDispatcher* eventDispatcher, WindowAttributeManager* attributeManager) :
    context{NULL}, title{u""},
    shutdown{false}, focused{false}, globalMouseX{0}, globalMouseY{0},
    moving{false}, moveOffX{0}, moveOffY{0}, resizing{false}, resizeX{0}, resizeY{0},
    maximized{false}, maximizeX{0}, maximizeY{0}, maximizeWidth{0}, maximizeHeight{0}, editIndex{0},
    selectedComponent{NULL}, cursorType{constants::CURSOR_DEFAULT},
    Container("window", "window", "window", false, eventDispatcher, attributeManager) {//TODO let you set a name

    this->eventDispatcher->addEventListener("close", std::bind(&Window::onClose, this, std::placeholders::_1));
    this->eventDispatcher->addEventListener("maximize", std::bind(&Window::onMaximize, this, std::placeholders::_1));
    this->eventDispatcher->addEventListener("unmaximize", std::bind(&Window::onUnmaximize, this, std::placeholders::_1));
    this->eventDispatcher->addEventListener("minimize", std::bind(&Window::onMinimize, this, std::placeholders::_1));
    this->eventDispatcher->addEventListener("unminimize", std::bind(&Window::onUnminimize, this, std::placeholders::_1));
    this->eventDispatcher->addEventListener("focus", std::bind(&Window::onFocus, this, std::placeholders::_1));
    this->eventDispatcher->addEventListener("blur", std::bind(&Window::onBlur, this, std::placeholders::_1));

    this->eventDispatcher->addEventListener("mousedown", std::bind(&Window::onMouseDown, this, std::placeholders::_1));
    this->eventDispatcher->addEventListener("keydown", std::bind(&Window::onKeyDown, this, std::placeholders::_1));
}

void oui::Window::initializeWindow(int width, int height) {
    graphics = createGraphics(width, height);
    graphics->setFont(Font::getFont(u"notoserif", 12, this));
    window = this;

    width = width / 2;
    height = height / 2;
    setAttribute("width-offset", width);
    setAttribute("height-offset", height);
    int widthOffset = width;
    int heightOffset = height;
    

    Panel* windowBar = new Panel("window-bar", "");
    windowBar->setAttribute("permanent", true);
    windowBar->setAttribute("cursor", u"pointer");
    windowBar->setAttribute("cursor", u"pointer");
    windowBar->parseAttribute("size", u"100 0 0 26");
    windowBar->setAttribute("z-index", 10);
    windowBar->setAttribute("bg-color1", Color(208, 208, 208, 255));
    windowBar->setAttribute("bg-color2", Color(208, 208, 208, 255));
    windowBar->setAttribute("bg-color1", Color::BLACK);
    windowBar->setAttribute("bg-color2", Color::BLACK);

    windowBar->addEventListener("mousedown", [this](ComponentEvent* e) {
        auto comp = e->getTarget();
        if (maximized) {
            onUnmaximize();

            // TODO if maximized?
            moveOffX = getWidth() / 2;
            moveOffY = comp->getHeight() / 2;
            parseAttribute("location", u"0 0 " + intToString(globalMouseX - moveOffX) + u" " + intToString(globalMouseY - moveOffY));
        } else {
            moveOffX = getMouseX();
            moveOffY = getMouseY();
        }
        moving = true;
    });

    windowBar->addEventListener("mouseup", [this](ComponentEvent* e) {
        moving = false;
        resizing = false;
    });

    Label* titleLbl = new Label("window-title", "windowText");
    titleLbl->parseAttribute("location", u"0 50 4 0");
    titleLbl->parseAttribute("font", u"notoserif 14");
    titleLbl->setAttribute("centered-y", true);
    titleLbl->setAttribute("auto-size", true);
    titleLbl->setAttribute("interactable", false);

    Button* minimizeBtn = new Button("minimizeBtn", "window-button");
    minimizeBtn->addEventConsumer("mousedown");

    minimizeBtn->addEventListener("click", [this](ComponentEvent* e) {
        WindowAttributeManager* attributeManager = this->getAttributeManager();
        if (this->minimized) {
            onSystemUnminimize();
        } else {
            onSystemMinimize();
        }
        e->stopPropagation();
    });
    minimizeBtn->parseAttribute("location", u"100 0 -73 0");
    minimizeBtn->parseAttribute("size", u"0 0 25 25");
    minimizeBtn->setAttribute("bg-color1", Color(200, 200, 200, 255));
    minimizeBtn->setAttribute("bg-color2", Color(200, 200, 200, 255));
    minimizeBtn->setAttribute("border-style", u"solid");
    minimizeBtn->setAttribute("image", u"minimize-btn.png");

    Button* maximizeBtn = new Button("maximizeBtn", "window-button");
    maximizeBtn->addEventConsumer("mousedown");
    maximizeBtn->addEventListener("click", [this](ComponentEvent* e) {
        WindowAttributeManager* attributeManager = this->getAttributeManager();
        if (this->maximized) {
            onSystemUnmaximize();
        } else {
            onSystemMaximize();
        }
    });
    maximizeBtn->parseAttribute("location", u"100 0 -49 0");
    maximizeBtn->parseAttribute("size", u"0 0 25 25");
    maximizeBtn->setAttribute("bg-color1", Color(200, 200, 200, 255));
    maximizeBtn->setAttribute("bg-color2", Color(200, 200, 200, 255));
    maximizeBtn->setAttribute("border-style", u"solid");
    maximizeBtn->setAttribute("image", u"maximize-btn1.png");

    Button* closeBtn = new Button("closeBtn", "window-button");
    closeBtn->addEventListener("click", [this](ComponentEvent* e) {
        WindowEvent* windowEvent = WindowEvent::create("close", this, getX(), getY(), getWidth(), getHeight());
        eventDispatcher->dispatchEvent(windowEvent);
        e->stopPropagation();
        delete windowEvent;
    });

    closeBtn->parseAttribute("location", u"100 0 -25 0");
    closeBtn->parseAttribute("size", u"0 0 25 25");
    closeBtn->setAttribute("bg-color1", Color(200, 200, 200, 255));
    closeBtn->setAttribute("bg-color2", Color(200, 200, 200, 255));
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

    menu->addEventListener("click", [this, menu](ComponentEvent* e) {
        menu->setAttribute("visible", false);
        setSelectedComponent(menu->getTarget());
    });
    addChild(menu);

    setAttribute("bg-color1", Color(255, 255, 255, 255));
    setAttribute("bg-color2", Color(255, 255, 255, 255));
    setAttribute("border-style", u"solid");
    getAttributeManager()->setProfile(u"default");
}

void oui::Window::setContext(Context* context) {
    this->context = context;
}

oui::Context* oui::Window::getContext() {
    return this->context;
}

bool oui::Window::isWindow() {
    return true;
}

int oui::Window::process() {
    if (shutdown) {
        return -1;
    }

    OS()->getGlobalMousePos(this, globalMouseX, globalMouseY);

    if (moving) {
        int newX = globalMouseX - moveOffX;
        int newY = globalMouseY - moveOffY;
        int xOffset = attributeManager->getXOffset();
        int yOffset = attributeManager->getYOffset();
        if (newX != xOffset || newY != yOffset) {
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

    Container::process();

    for (int i = 0; i < processableChildren.size(); i++) {
        auto child = processableChildren.at(i);
        if (child->needsProcessing) {
            child->process();
        }
    }

    if (needsGraphicsUpdate()) {
        //TODO needsGraphicsUpdate is called because of DrawablePanel so profile doesn't need to change...
        refreshProfile();
        redraw();
        render();
    }

    return 0;
}

oui::Graphics* oui::Window::getGraphics() {
    return graphics;
}

void oui::Window::onSystemClose(ComponentEvent* compEvent) {
    WindowEvent* event = new WindowEvent("close", this, getX(), getY(), getWidth(), getHeight());
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemMinimize(ComponentEvent* compEvent) {
    WindowEvent* event = new WindowEvent("minimize", this, getX(), getY(), getWidth(), getHeight());
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemUnminimize(ComponentEvent* compEvent) {
    WindowEvent* event = new WindowEvent("unminimize", this, getX(), getY(), getWidth(), getHeight());
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemMaximize(ComponentEvent* compEvent) {
    WindowEvent* event = new WindowEvent("maximize", this, getX(), getY(), getWidth(), getHeight());
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemUnmaximize(ComponentEvent* compEvent) {
    WindowEvent* event = new WindowEvent("unmaximize", this, getX(), getY(), getWidth(), getHeight());
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemFocus(ComponentEvent* compEvent) {
    WindowEvent* event = new WindowEvent("focus", this, getX(), getY(), getWidth(), getHeight());
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemBlur(ComponentEvent* compEvent) {
    WindowEvent* event = new WindowEvent("blur", this, getX(), getY(), getWidth(), getHeight());
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemMouseMove(ComponentEvent* compEvent) {
    MouseEvent* rawMouseEvent = static_cast<MouseEvent*>(compEvent);
    mouseX = rawMouseEvent->windowX;
    mouseY = rawMouseEvent->windowY;
    MouseEvent* event = MouseEvent::create("mousemove", true, this, rawMouseEvent->button, rawMouseEvent->buttons, rawMouseEvent->movementX, rawMouseEvent->movementY);

    if (!resizing) {
        // TODO math util
        int dist = (int) abs(sqrt(pow(globalMouseX - getX() - getWidth(), 2) + pow(globalMouseY - getY() - getHeight(), 2)));
        if (dist < 15) {
            setCursor(constants::CURSOR_RESIZE);
        } else {
            setCursor(event->getTarget()->getCursor());
        }
    }


	setHovered(false);
	event->getTarget()->setHovered(true);

    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemMouseUp(ComponentEvent* compEvent) {
    MouseEvent* rawMouseEvent = static_cast<MouseEvent*>(compEvent);
    mouseX = rawMouseEvent->windowX;
    mouseY = rawMouseEvent->windowY;

    MouseEvent* event = MouseEvent::create("mouseup", true, this, rawMouseEvent->button, rawMouseEvent->buttons, rawMouseEvent->movementX, rawMouseEvent->movementY);
    Component* c = event->getTarget();
    bool sendClickEvent = c->isMouseDown();
    moving = false;
    resizing = false;
    setMouseDown(false);
    eventDispatcher->dispatchEvent(event);
    delete event;
    
    // Fire click event
    if (sendClickEvent) {
        MouseEvent* clickEvent = MouseEvent::create("click", true, this, rawMouseEvent->button, rawMouseEvent->buttons, rawMouseEvent->movementX, rawMouseEvent->movementY);
        eventDispatcher->dispatchEvent(clickEvent);
        delete clickEvent;
    }
}


void oui::Window::onSystemMouseDown(ComponentEvent* compEvent) {
    MouseEvent* rawMouseEvent = static_cast<MouseEvent*>(compEvent);
    mouseX = rawMouseEvent->windowX;
    mouseY = rawMouseEvent->windowY;
    MouseEvent* event = MouseEvent::create("mousedown", true, this, rawMouseEvent->button, rawMouseEvent->buttons, rawMouseEvent->movementX, rawMouseEvent->movementY);
    Component* comp = event->getTarget();

    Menu* menu = static_cast<Menu*>(getChild("rightClickMenu"));
    if (menu != NULL && comp != menu) {
        menu->setAttribute("visible", false);
    }

    switch(event->button) {
        case 0: // Left Click
            int distToResize = (int) abs(sqrt(pow(globalMouseX - getX() - getWidth(), 2) + pow(globalMouseY - getY() - getHeight(), 2)));
            if (distToResize < 15) {
                closeRightClickMenu();
                resizeX = getWidth() - (globalMouseX - getX());
                resizeY = getHeight() - (globalMouseY - getY());
                resizing = true;
                return;
            }
            break;
    }

    window->setMouseDown(false);
    comp->setMouseDown(true);
    setSelectedComponent(comp);
    
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemScrollWheel(ComponentEvent* compEvent) {
    ScrollEvent* rawScrollEvent = static_cast<ScrollEvent*>(compEvent);
    ScrollEvent* event = ScrollEvent::create(true, this, rawScrollEvent->buttons, rawScrollEvent->movementX, rawScrollEvent->movementY, rawScrollEvent->scrollDistance);
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemKeyDown(ComponentEvent* compEvent) {
    KeyboardEvent* event = KeyboardEvent::create("keydown", true, this, static_cast<KeyboardEvent*>(compEvent)->key);
    eventDispatcher->dispatchEvent(event);
    delete event;
    onSystemKeyTyped(compEvent);
}

void oui::Window::onSystemKeyUp(ComponentEvent* compEvent) {
    KeyboardEvent* event = KeyboardEvent::create("keyup", true, this, static_cast<KeyboardEvent*>(compEvent)->key);
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onSystemKeyTyped(ComponentEvent* compEvent) {
    KeyboardEvent* event = KeyboardEvent::create("keytyped", true, this, static_cast<KeyboardEvent*>(compEvent)->key);
    eventDispatcher->dispatchEvent(event);
    delete event;
}

void oui::Window::onClose(ComponentEvent* compEvent) {
    this->shutdown = true;
}

void oui::Window::onMinimize(ComponentEvent* compEvent) {
    // Move this to SDL window
    minimized = true;
}

void oui::Window::onUnminimize(ComponentEvent* compEvent) {
    // Move this to SDL window
    minimized = false;
}

void oui::Window::onMaximize(ComponentEvent* compEvent) {
    Button* maximizeBtn = static_cast<Button*>(((Container*) getChild("window-bar"))->getChild("maximizeBtn"));
    if (maximizeBtn != NULL) {
        maximizeBtn->setAttribute("image", u"maximize-btn2.png");
    }
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
    refreshProfile();
}

void oui::Window::onUnmaximize(ComponentEvent* compEvent) {
    Button* maximizeBtn = static_cast<Button*>(((Container*) getChild("window-bar"))->getChild("maximizeBtn"));
    if (maximizeBtn != NULL) {
        maximizeBtn->setAttribute("image", u"maximize-btn1.png");
    }
    maximized = false;

    parseAttribute("location", u"0 0 " + intToString(maximizeX) + u" " + intToString(maximizeY));
    parseAttribute("size", u"0 0 " + intToString(maximizeWidth) + u" " + intToString(maximizeHeight));
    refreshProfile();
}

void oui::Window::onFocus(ComponentEvent* compEvent) {
    focused = true;
}

void oui::Window::onBlur(ComponentEvent* compEvent) {
    focused = false;
}

void oui::Window::onMouseDown(ComponentEvent* compEvent) {
    MouseEvent* event = static_cast<MouseEvent*>(compEvent);
    Component* c = event->getTarget();
    if (event->button == 3) {
        Menu* menu = static_cast<Menu*>(getChild("rightClickMenu"));
        std::vector<std::u16string> rcOptions = c->getRightClickOptions();
        if (menu != NULL && rcOptions.size() != 0) {
            setSelectedComponent(c);

            std::vector<Attribute> options;
            for (int i = 0; i < rcOptions.size(); i++) {
                options.push_back(rcOptions[i]);
            }

#ifdef _DEBUG
            options.push_back(u" Inspect");
#endif

            menu->setAttribute("options", options);
            if (mouseX + menu->getWidth() > getWidth()) {
                mouseX = getWidth() - menu->getWidth();
            }
            if (mouseY + menu->getHeight() > getHeight()) {
                mouseY -= menu->getHeight();
            }
            menu->parseAttribute("location", u"0 0 " + intToString(mouseX) + u" " + intToString(mouseY));
            menu->setAttribute("visible", true);
            menu->setTarget(c);
        }
    }
}

void oui::Window::onKeyDown(ComponentEvent* compEvent) {
    int code = static_cast<KeyboardEvent*>(compEvent)->key;
    if (code == KEY_Z && isCtrlDown()) {
        undo();
    } else if (code == KEY_Y && isCtrlDown()) {
        redo();
    }
}

void oui::Window::redraw() {
    bool visible = getAttributeManager()->isVisible();
    if (visible) {
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

void oui::Window::setTitle(const std::u16string& title) {
    if (this->title == title) {
        return;
    }
    this->title = title;
    
    if (getChild("window-bar") != NULL) {
        Label* titleLbl = static_cast<Label*>(((Container*) getChild("window-bar"))->getChild("window-title"));
        if (titleLbl != NULL) {
            titleLbl->setAttribute("text", title);
        }
    }
}

bool oui::Window::setCursor(std::u16string cursor) {
    if (this->cursorType == cursor) {
        return false;
    }
    this->cursorType = cursor;
    return true;
}

bool oui::Window::hasFocus() {
    return focused;
}

void oui::Window::addEditEvent(EditEvent* e, bool append) {
    if (editIndex != editEvents.size() - 1) {
        std::vector<EditEvent*>::iterator it;
        for (it = editEvents.begin() + editIndex; it != editEvents.end(); ++it) {
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
        editIndex = (int) editEvents.size() - 1;
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
            editIndex = (int) editEvents.size() - 1;
        }
    }
}


bool oui::Window::isAltDown() {
    return false;
}
bool oui::Window::isCtrlDown() {
    return false;
}
bool oui::Window::isMetaDown() {
    return false;
}
bool oui::Window::isShiftDown() {
    return false;
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

void oui::Window::closeRightClickMenu() {
    Component* menu = getChild("rightClickMenu");
    if (menu != NULL && menu->getAttributeManager()->isVisible()) {
        menu->setAttribute("visible", false);
    }
}

std::u16string oui::Window::getTitle() {
    return title;
}

void oui::Window::setVisible(bool visible) {

    if (visible) {
        attributeManager->setProfile(u"default");
    }

    setAttribute("visible", visible);
}


void oui::Window::setPage(std::u16string path) {
	oui::ComponentLoader cl;
	cl.loadComponents(path);
	this->removeAllChildren();
	this->addChild(cl.toPanel());
}

void oui::Window::setClipboardText(const std::u16string& text) {
    SDL_SetClipboardText(convertUTF16toUTF8(text).c_str());//TODO clipboar should support wstring
}

std::u16string oui::Window::getClipboardText() {
    return convertUTF8toUTF16(SDL_GetClipboardText());
}
bool oui::Window::hasClipboardText() {
    return SDL_HasClipboardText();
}

oui::Window* oui::Window::getWindow() {
    return static_cast<Window*>(parent);
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

oui::WindowAttributeManager* oui::Window::getAttributeManager() {
    return static_cast<WindowAttributeManager*>(attributeManager);
}