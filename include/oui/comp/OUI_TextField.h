#ifndef OUI_TEXTFIELD_H
#define OUI_TEXTFIELD_H

#include "OUI_Export.h"
#include "oui/comp/OUI_Component.h"

namespace oui {

    class TextField : public Component {

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

        public: OUI_API ~TextField();
        public: OUI_API TextField(const std::string& name, const std::string& classes);

        public: OUI_API void addedToContainer(Container* container) override;

        public: OUI_API int process() override;

        public: OUI_API void setProfile(const std::u16string& profile) override;
        public: OUI_API std::vector<std::u16string> getRightClickOptions() override;

        public: OUI_API void redraw() override;
        public: OUI_API void handleEvent(Event& e) override;
        public: OUI_API void setSelected(bool selected) override;

        public: OUI_API void setText(const std::u16string& text);

        public: OUI_API void insertString(const std::u16string& string);
        public: OUI_API void insertChar(char c);
        public: OUI_API void deleteChar(bool backspace);
        public: OUI_API void moveCarat(bool right);

        public: OUI_API int getCaratIndex();
        public: OUI_API void setCaratIndex(int index);

        public: OUI_API void updateTextPosition();

        private: int getIndexAt(int x);
        private: std::function<void()> getUndoEvent();


    };
}
#endif