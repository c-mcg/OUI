
#include "event/OUI_EditEvent.h"
#include "util/OUI_StringUtil.h"

oui::EditEvent::EditEvent(std::function<void()> undoHandler, std::function<void()> redoHandler, bool hasPerformed, bool appendable, Component* source)
    : undoHandler{undoHandler}, redoHandler{redoHandler},
    performed{hasPerformed}, appendable{appendable}, source{source} {
    if (appendable && source == NULL) {
        //error
    }
}


void oui::EditEvent::combine(EditEvent e) {
    std::function<void()> redoHandler1 = std::function<void()>(redoHandler);
    std::function<void()> redoHandler2 = std::function<void()>(e.redoHandler);
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