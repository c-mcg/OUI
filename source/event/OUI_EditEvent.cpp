
#include "event/OUI_EditEvent.h"
#include "util/OUI_StringUtil.h"
#include "exception/OUI_ArgumentException.h"

oui::EditEvent::EditEvent(std::function<void()> undoHandler, std::function<void()> redoHandler, bool hasPerformed, bool appendable, Component* source)
    : undoHandler{undoHandler}, redoHandler{redoHandler},
    performed{hasPerformed}, appendable{appendable}, source{source} {
    if (appendable && source == NULL) {
        throw ArgumentException(
            "EditEvent",
            "constructor",
            "Appendable edit event created with no source component",
            "Specify a source component"
        );
    }
}


void oui::EditEvent::combine(EditEvent e) {
    auto redoHandler1 = std::function<void()>(redoHandler);
    auto redoHandler2 = std::function<void()>(e.redoHandler);
    this->redoHandler = [redoHandler1, redoHandler2] {
        redoHandler1();
        redoHandler2();
    };
}

bool oui::EditEvent::hasPerformed() {
    return performed;
}
void oui::EditEvent::performUndo() {
    undoHandler();
    performed = false;
}
void oui::EditEvent::performRedo() {
    redoHandler();
    performed = true;
}
bool oui::EditEvent::isAppendable() {
    return appendable;
}
oui::Component* oui::EditEvent::getSource() {
    return source;
}