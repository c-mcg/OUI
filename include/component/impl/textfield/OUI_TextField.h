#ifndef OUI_TEXTFIELD_H
#define OUI_TEXTFIELD_H

#include "OUI_Export.h"
#include "component/OUI_Component.h"
#include "component/impl/textfield/OUI_TextFieldAttributeManager.h"

namespace oui {

    class OUI_API TextField : public Component {

        public:
        
            ~TextField();
            TextField(const std::string& name, const std::string& classes, EventDispatcher* eventDispatcher=new EventDispatcher(), TextFieldAttributeManager* attributeManager=new TextFieldAttributeManager());

            void addedToContainer(Container* container) override;

            int process() override;

            std::vector<std::u16string> getRightClickOptions() override;

            void redraw() override;
            void setSelected(bool selected) override;

            void setText(const std::u16string& text);

            void insertString(const std::u16string& string);
            void insertChar(char c);
            void deleteChar(bool backspace);
            void moveCarat(bool right);

            int getCaratIndex();
            void setCaratIndex(int index);

            void updateTextPosition();
        
            virtual TextFieldAttributeManager* getAttributeManager() override;

        private:

            int drawX;
            int selectStart;
            bool caratVisible;
            long long lastCaratSwitch;
            int caratIndex;

            bool highlighting;

            bool resetInput;
            bool typing;//true if last input was typing, false if last input was deleting
            long long lastInput;
            
            int getIndexAt(int x);
            std::function<void()> getUndoEvent();

            void onMenuOption(ComponentEvent* e);
            void onMouseDown(ComponentEvent* e);
            void onMouseUp(ComponentEvent* e);
            void onMouseMove(ComponentEvent* e);
            void onKeyTyped(ComponentEvent* e);
    };
}
#endif