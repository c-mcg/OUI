#ifndef OUI_EDITEVENT_H
#define OUI_EDITEVENT_H

#include "OUI_Export.h"
#include "components/OUI_Component.h"
#include <functional>

namespace oui {

    class EditEvent : public ComponentEvent {

        protected: bool performed;
        protected: std::function<void()> undoHandler;
        protected: std::function<void()> redoHandler;

        private: bool appendable;
        private: Component* source;

        public: OUI_API EditEvent(std::function<void()> undoHandler, std::function<void()> redoHandler, bool hasPerformed = false, bool appendable = false, Component* source = NULL);

        public: OUI_API void combine(EditEvent e);
        public: OUI_API bool hasPerformed();
        public: OUI_API void performUndo();
        public: OUI_API void performRedo();
        public: OUI_API bool isAppendable();
        public: OUI_API Component* getSource();

    };

}
#endif
