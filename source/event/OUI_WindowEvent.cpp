#include "event/OUI_WindowEvent.h"

#include "OUI_window.h"

oui::WindowEvent* oui::WindowEvent::create(std::string type, Window* window, int newX, int newY, int newWidth, int newHeight) {
    return new WindowEvent(type, window, newX, newY, newWidth, newHeight);
}

oui::WindowEvent::WindowEvent(std::string type, Window* window, int newX, int newY, int newWidth, int newHeight):
    window{window}, newX{newX}, newY{newY}, newWidth{newWidth}, newHeight{newHeight},
    ComponentEvent(window, "window", type, false)
{

}