#ifndef OUI_COMPONENTLOADER_H
#define OUI_COMPONENTLOADER_H

#include "OUI_Export.h"
#include "osal/OSAL_Sheet.h"
#include <functional>

namespace oui {

	class Component;
	class Panel;

	class OUI_API ComponentLoader {
		private: static std::unordered_map<std::string, std::function<Component*(std::string, std::string, std::vector<std::string>, std::vector<std::u16string>)>>  tags;

		//Use this to add handlers for tags, returns false if the tag is already added
		//When the tag is found, the loader adds the result of createComponent to the component list
		//createComponent parameters are (std::u16string name, string classes, vector<std::u16string> attributeNames, vector<std::u16string> attributeValues)
		//createComponent can return NULL and can be used for loading any XML data
		public: static bool addTag(std::string tag, std::function<Component*(std::string, std::string, std::vector<std::string>, std::vector<std::u16string>)> createComponent);

		public: std::vector<Component*> components;
		private: OSAL::Sheet attributes;

		public: ComponentLoader();

		public: bool loadComponents(std::u16string path);
		public: bool applyAttributes(std::u16string path);
		public: Panel* toPanel();

	};
}
#endif