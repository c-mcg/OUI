#ifndef OSAL_SHEET_H
#define OSAL_SHEET_H

#include "osal/OSAL_Element.h"

/*
* This class represents a sheet full of elements with attributes
*/
namespace OSAL {
	class Sheet {
		private: std::u16string name;//The name of the sheet
		private: std::vector<Element> elements;//The elements in the sheet

		public: Sheet();
		public: Sheet(const std::u16string& name, const std::vector<Element>& elements);

		public: Sheet combine(Sheet sheet);
		public: bool isValid();

		public: std::u16string getName();
		public: Element getElement(Element::Type type, const std::string& name);
		public: Element getElement(const std::string& name);//prefix with '#', '.' or nothing

		public: std::vector<Element> getElements();

	};
}

#endif