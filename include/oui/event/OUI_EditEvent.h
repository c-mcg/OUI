#ifndef OUI_EDITEVENT_H
#define OUI_EDITEVENT_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Component.h"
#include <functional>

namespace oui {

    class OUI_API EditEvent {

        protected: bool performed;
        protected: std::function<void()> undoHandler;
        protected: std::function<void()> redoHandler;

        private: bool appendable;
        private: Component* source;

        public: EditEvent(std::function<void()> undoHandler, std::function<void()> redoHandler, bool hasPerformed = false, bool appendable = false, Component* source = NULL);

        public: void combine(EditEvent e);
        public: bool hasPerformed();
        public: void performUndo();
        public: void performRedo();
        public: bool isAppendable();
        public: Component* getSource();

    };

}
#endif
