#include <SDL.h>

#include "OUI.h"
#include "sdl2/OUI_SDL_Window.h"
#include "sdl2/OUI_SDL_Graphics.h"
#include "OUI_KeyCodes.h"

oui::SDLWindow::~SDLWindow() {
	SDL_DestroyWindow(baseWindow);
	SDL_DestroyRenderer(renderer);
}

oui::SDLWindow::SDLWindow(int width, int height) : Window(width, height) {
    // TODO retina: https://stackoverflow.com/questions/18544881/sdl-2-0-retina-mac
    this->baseWindow = SDL_CreateWindow(convertUTF16toUTF8(getTitle()).c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
	if (baseWindow == NULL) {
		std::cout << "NULL WINDOW!!" << std::endl;
	}
    this->renderer = SDL_CreateRenderer(baseWindow, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
		std::cout << "NULL RENDERER" << std::endl;
		//TODO error
	}

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
void oui::SDLWindow::minimize() {
    std::cout << "Window minimized" << std::endl;
    Window::minimize();
    SDL_MinimizeWindow(baseWindow);
}
void oui::SDLWindow::maximize() {
    Window::maximize();
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
						baseWindowId, event->window.event);
				break;
		}
	} else if (event->type == SDL_KEYMAPCHANGED) {
        std::cout << "keys changed: " << event->key.keysym.sym << std::endl;
    } else if (event->type == SDL_TEXTEDITING) {
        std::cout << "SDL_TEXTEDITING: " << event->key.keysym.sym << std::endl;
    }

    //TODO native text events: https://wiki.libsdl.org/SDL_TextInputEvent

	if (event->type == SDL_MOUSEBUTTONUP && event->button.windowID == baseWindowId) {
		handleMouseUpEvent(MouseEvent(Event::MOUSE_UP, event->button.x, event->button.y, globalMouseX, globalMouseY, event->button.button));
	}

	if (event->type == SDL_MOUSEBUTTONDOWN && (event->button.windowID == baseWindowId || focused)) {
		handleMouseDownEvent(MouseEvent(Event::MOUSE_DOWN, event->button.x, event->button.y, globalMouseX, globalMouseY, event->button.button));
    }

	if (event->type == SDL_MOUSEMOTION && (event->motion.windowID == baseWindowId || focused)) {
		handleMouseMoveEvent(MouseEvent(Event::MOUSE_MOVE, event->motion.x, event->motion.y, globalMouseX, globalMouseY));
	}

    if (event->type == SDL_FINGERDOWN) {
        std::cout << "FINGER event!: " << std::endl;
    }

    if (event->type == SDL_MULTIGESTURE) {
        // std::cout << "MULTIGESTURE event!: " << std::endl;
    }

	if (event->type == SDL_MOUSEWHEEL && (event->wheel.windowID == baseWindowId || focused)) {
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

	if (event->type == SDL_KEYDOWN && (event->key.windowID == baseWindowId || focused)) {
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
	if (event->type == SDL_KEYUP && (event->key.windowID == baseWindowId || focused)) {
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