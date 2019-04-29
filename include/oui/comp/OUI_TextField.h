#ifndef OUI_TEXTFIELD_H
#define OUI_TEXTFIELD_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Component.h"

namespace oui {

    class OUI_API TextField : public Component {

        private: Color textColor;
        private: Font* font;
        //TODO getters and setters
        private: Color caratColor;
        private: int caratWidth;
        private: int caratHeightOffset;
        private: Color highlightColor;

        private: int selectStart;
        private: bool caratVisible;
        private: long long lastCaratSwitch;
        private: int caratIndex;
        private: std::u16string text;

        private: bool highlighting;

        private: bool resetInput;
        private: bool typing;//true if last input was typing, false if last input was deleting
        private: long long lastInput;

        private: int drawX;

        public: ~TextField();
        public: TextField(const std::string& name, const std::string& classes);

        public: void addedToContainer(Container* container) override;

        public: int process() override;

        public: void setProfile(const std::u16string& profile) override;
        public: std::vector<std::u16string> getRightClickOptions() override;

        public: void redraw() override;
        public: void handleEvent(Event& e) override;
        public: void setSelected(bool selected) override;

        public: void setText(const std::u16string& text);

        public: void insertString(const std::u16string& string);
        public: void insertChar(char c);
        public: void deleteChar(bool backspace);
        public: void moveCarat(bool right);

        public: int getCaratIndex();
        public: void setCaratIndex(int index);

        public: void updateTextPosition();

        private: int getIndexAt(int x);
        private: std::function<void()> getUndoEvent();


    };
}
#endif