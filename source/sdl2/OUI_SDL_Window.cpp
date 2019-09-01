#include <SDL.h>

#include "OUI.h"
#include "sdl2/OUI_SDL_Window.h"
#include "sdl2/OUI_SDL_Graphics.h"
#include "OUI_KeyCodes.h"

#include "event/OUI_ScrollEvent.h"
#include "event/OUI_KeyboardEvent.h"

oui::SDLWindow::~SDLWindow() {
    SDL_DestroyWindow(baseWindow);
    SDL_DestroyRenderer(renderer);
}

oui::SDLWindow::SDLWindow(int width, int height) :
    ctrlDown{false}, shiftDown{false}, altDown{false}, metaDown{false},
    Window(width, height) {
    // TODO retina: https://stackoverflow.com/questions/18544881/sdl-2-0-retina-mac
    
    this->baseWindow = SDL_CreateWindow(convertUTF16toUTF8(getTitle()).c_str(), 0, 0, width, height, SDL_WINDOW_BORDERLESS);
    this->renderer = SDL_CreateRenderer(baseWindow, -1, SDL_RENDERER_ACCELERATED);
    this->baseWindowId = SDL_GetWindowID(baseWindow);
    this->baseCursor = NULL;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    initializeWindow(width, height);
}

oui::Graphics* oui::SDLWindow::createGraphics(int width, int height) {
    Graphics* g = new SDLGraphics(width, height, renderer);
    return g;
}

void oui::SDLWindow::render() {
    SDL_RenderPresent(renderer);
}
void oui::SDLWindow::setTitle(const std::u16string& title) {
    Window::setTitle(title);
    SDL_SetWindowTitle(baseWindow, convertUTF16toUTF8(title).c_str());
}
void oui::SDLWindow::showWindow() {
    SDL_ShowWindow(baseWindow);
}
void oui::SDLWindow::hideWindow() {
    SDL_HideWindow(baseWindow);
}
void oui::SDLWindow::setSize(int width, int height) {
    SDL_SetWindowSize(baseWindow, width, height);
    
}
void oui::SDLWindow::setPosition(int x, int y) {
    SDL_SetWindowPosition(baseWindow, x, y);
}
bool oui::SDLWindow::setCursor(int cursor) {
    if (!Window::setCursor(cursor)) {
        return false;
    }
    SDL_Cursor* baseCursor;
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
    baseCursor = SDL_CreateSystemCursor(newCursor);
    SDL_SetCursor(baseCursor);
    this->baseCursor = baseCursor;
    return true;
}
void oui::SDLWindow::onMinimize(ComponentEvent* event) {
    Window::onMaximize(event);
    SDL_MinimizeWindow(baseWindow);
}
void oui::SDLWindow::onMaximize(ComponentEvent* event) {
    Window::onMaximize(event);
    SDL_MaximizeWindow(baseWindow);
}

void oui::SDLWindow::handleSDLEvent(SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT && event->window.windowID == baseWindowId) {

        switch (event->window.event) {

            case SDL_WINDOWEVENT_SHOWN:
                std::cout << "window shown!!" <<std::endl;
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                std::cout << "window hidden!!" <<std::endl;
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                //std::cout << "window exposed!!" <<std::endl;
                break;
            case SDL_WINDOWEVENT_MOVED:
                //std::cout << "window moved!!" <<std::endl;
                break;
            case SDL_WINDOWEVENT_RESIZED:
                std::cout << "window resize!!" <<std::endl;
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                // This gets called when we change the size
                std::cout << "window size changed!!" <<std::endl;
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
                onSystemClose();
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
                        baseWindowId, event->window.event);
                break;
        }
    } else if (event->type == SDL_KEYMAPCHANGED) {
        std::cout << "keys changed: " << event->key.keysym.sym << std::endl;
    } else if (event->type == SDL_TEXTEDITING) {
        std::cout << "SDL_TEXTEDITING: " << event->key.keysym.sym << std::endl;
    }

    //TODO native text events: https://wiki.libsdl.org/SDL_TextInputEvent

    if (event->type == SDL_MOUSEMOTION && (event->motion.windowID == baseWindowId || focused)) {
        MouseEvent* mouseEvent = new MouseEvent("mousemove", true, NULL, false, event->button.button, mouseButtonsDown, 0, 0, false, false, event->button.x - lastMouseX, event->button.x - lastMouseX, globalMouseX, globalMouseY, false, event->motion.x, event->motion.y);
        onSystemMouseMove(mouseEvent);
        delete mouseEvent;
    }

    if (event->type == SDL_MOUSEBUTTONUP && event->button.windowID == baseWindowId) {
        auto it =  std::find(mouseButtonsDown.begin(), mouseButtonsDown.end(), event->button.button);
        if (it != mouseButtonsDown.end()) {
            mouseButtonsDown.erase(it);
        }

        MouseEvent* mouseEvent = new MouseEvent("mouseup", true, NULL, false, event->button.button, mouseButtonsDown, 0, 0, false, false, event->button.x - lastMouseX, event->button.x - lastMouseX, globalMouseX, globalMouseY, false, event->button.x, event->button.y);
        onSystemMouseUp(mouseEvent);
        delete mouseEvent;

        lastMouseX = event->button.x;
        lastMouseY = event->button.y;
    }

    if (event->type == SDL_MOUSEBUTTONDOWN && (event->button.windowID == baseWindowId || focused)) {
        auto it = std::find(mouseButtonsDown.begin(), mouseButtonsDown.end(), event->button.button);
        if (it == mouseButtonsDown.end()) {
            mouseButtonsDown.push_back(event->button.button);
        }
        MouseEvent* mouseEvent = new MouseEvent("mousedown", true, NULL, false, event->button.button - 1, mouseButtonsDown, 0, 0, false, false, event->button.x - lastMouseX, event->button.x - lastMouseX, globalMouseX, globalMouseY, false, event->button.x, event->button.y);
        onSystemMouseDown(mouseEvent);
        delete mouseEvent;
        lastMouseX = event->button.x;
        lastMouseY = event->button.y;
    }

    if (event->type == SDL_FINGERDOWN) {
        std::cout << "FINGER event!: " << std::endl;
    }

    if (event->type == SDL_MULTIGESTURE) {
        // std::cout << "MULTIGESTURE event!: " << std::endl;
    }

    if (event->type == SDL_MOUSEWHEEL && (event->wheel.windowID == baseWindowId || focused)) {
        ScrollEvent* scrollEvent = new ScrollEvent(true, NULL, false, mouseButtonsDown, 0, 0, false, false, 0, 0, globalMouseX, globalMouseY, false, getMouseX(), getMouseY(), event->wheel.y);
        onSystemScrollWheel(scrollEvent);
        delete scrollEvent;
    }

    if (event->type == SDL_KEYDOWN && (event->key.windowID == baseWindowId || focused)) {
        int code = event->key.keysym.sym;
        switch(code) {
            case KEY_CONTROL_LEFT:
            case KEY_CONTROL_RIGHT:
                ctrlDown = true;
                break;

            case KEY_SHIFT_LEFT:
            case KEY_SHIFT_RIGHT:
                shiftDown = true;
                break;

            case KEY_ALT_LEFT:
            case KEY_ALT_RIGHT:
                altDown = true;
                break;

            case KEY_META:
                metaDown = true;
                break;
        }

        KeyboardEvent* keyboardEvent = new KeyboardEvent("keydown", true, NULL, false, false, false, false, code);
        onSystemKeyDown(keyboardEvent);
        delete keyboardEvent;
    }
    if (event->type == SDL_KEYUP && (event->key.windowID == baseWindowId || focused)) {
        int code = event->key.keysym.sym;
        switch(code) {
            case KEY_CONTROL_LEFT:
            case KEY_CONTROL_RIGHT:
                ctrlDown = false;
                break;

            case KEY_SHIFT_LEFT:
            case KEY_SHIFT_RIGHT:
                shiftDown = false;
                break;

            case KEY_ALT_LEFT:
            case KEY_ALT_RIGHT:
                altDown = false;
                break;

            case KEY_META:
                metaDown = true;
                break;
        }

        KeyboardEvent* keyboardEvent = new KeyboardEvent("keyup", true, NULL, false, false, false, false, code);
        onSystemKeyUp(keyboardEvent);
        delete keyboardEvent;
    }
    
}

bool oui::SDLWindow::isAltDown() {
    return altDown;
}
bool oui::SDLWindow::isCtrlDown() {
    return ctrlDown;
}
bool oui::SDLWindow::isMetaDown() {
    return metaDown;
}
bool oui::SDLWindow::isShiftDown() {
    return shiftDown;
}