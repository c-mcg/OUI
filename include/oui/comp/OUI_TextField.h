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
		public: TextField(std::string name, std::string classes);

		public: virtual void addedToContainer(Container* container);

		public: virtual int process();

		public: virtual void setProfile(std::u16string profile);
		public: virtual std::vector<std::u16string> getRightClickOptions();

		public: virtual void redraw();
		public: virtual void handleEvent(Event& e);
		public: virtual void setSelected(bool selected);

		public: void setText(std::u16string text);

		public: void insertString(std::u16string string);
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