#include "oui/attribute/OUI_AttributeProfile.h"

#include <iostream>//TODO remove
#include "OUI.h"//TODO remove

oui::AttributeProfile::~AttributeProfile() {
	
	//Loop through all attributes and delete
	for (auto it = attributes.begin(); it != attributes.end();) {
		//Attribute* a = it->second;
		it = attributes.erase(it);
		//delete a;
	}
	attributes.clear();

}

oui::AttributeProfile::AttributeProfile(std::string componentName, AttributeProfile* defaultProfile) {

	//Initialize class members
	attributes = std::unordered_map<std::string, Attribute>();
	if (componentName != "...") {
		std::cout << "'" << componentName.c_str() << "'" << std::endl;
	}
	this->componentName = componentName;
	if (defaultProfile != NULL) {
		this->defaultProfiles.push_back(defaultProfile);
	}
}

void oui::AttributeProfile::clearDefaultProfiles() {
	this->defaultProfiles.clear();
 }

void oui::AttributeProfile::addDefaultProfile(AttributeProfile* defaultProfile) {
	this->defaultProfiles.push_back(defaultProfile);
 }

void oui::AttributeProfile::parseAttribute(std::string name, std::u16string value) {

	//This parses the attribute to create an OSAL Attribute
	OSAL::Attribute osalAttribute = OSAL::Attribute(name, value.c_str());

	//Set the attribute, performing needed substitutions
	setAttribute(name, osalAttribute);

}

void oui::AttributeProfile::setAttribute(std::string name, OSAL::Attribute value)  {

	//Perform any substitutions
	//`AttributeSubstitution::applySubstitution` recursively calls this function with the substitutions
	if (AttributeSubstitution::applySubstitution(name, value, this)) {
		return;
	}

	//All attributes with multiple values should have been substituted away by now
	if (value.getNumValues() > 1) {
		//TODO error
	}

	//Delete the attribute if it already exists
	auto it = attributes.find(name);
	if (it != attributes.end()) {//TOOD deletion could be avoided if we reuse the attribute (does this affect speed?)
		//delete it->second;
		attributes.erase(it);
	}

	//Populate the new attribute value
	Attribute val = Attribute();
	switch (value.getType()) {
		case OSAL::TYPE_INT:
			val = value.getAsInt();
			break;
		case OSAL::TYPE_STRING:
			val = value.getAsString();
			break;
		case OSAL::TYPE_DOUBLE:
			val = value.getAsDouble();
			break;
		case OSAL::TYPE_BOOL:
			val = value.getAsBool();
			break;
	}
	
	//Finally, add the attribute to our attributes list
	attributes.insert({name, val});
}

void oui::AttributeProfile::setAttribute(std::string name, Attribute value) {

	//See if we can get a valid substitution
	if (AttributeSubstitution::hasSubstitution(name)) {

		//If the attribute is a string, we will put it back in automatically
		if (value.type == OSAL::TYPE_STRING) {
			setAttribute(name, OSAL::Attribute(name, value.stringVal));
			return;
		}

		//TODO throw error
		std::cout << "Use parseAttribute for attributes with multiple arguments name=" << name.c_str() << std::endl;

		return;
	}

	//Delete the attribute if it already exists
	auto it = attributes.find(name);
	if (it != attributes.end()) {

		//Return if the name with specified value already exists
		if (it->second.equals(value)) {
			return;
		}

		//Delete the attribute to make room for new value
		//delete it->second;
		attributes.erase(it);
	}

	std::cout << "added attribute " << name.c_str() << " value " << value << std::endl;
	//Create and insert the new value
	attributes.insert({name, value});

}

/*void oui::AttributeProfile::setAttribute(std::u16string name, Attribute* value) {

	//Delete the attribute if it already exists
	std::unordered_map<std::u16string, Attribute*>::iterator it = attributes.find(name);
	if (it != attributes.end()) {
		delete it->second;
		attributes.erase(it);
	}

	//Create and insert the new value
	attributes.insert({name, value});

}*/

oui::Attribute* oui::AttributeProfile::hasAttribute(std::string name) {

	auto it = attributes.find(name);

	if (it != attributes.end()) {
		return &(it->second);
	}

	for (int i = 0; i < defaultProfiles.size(); i++) {
		Attribute* attr = defaultProfiles.at(i)->hasAttribute(name);
		if (attr != NULL) {
			return attr;
		}
	}

	return NULL;
}

int oui::AttributeProfile::getInt(std::string name) {

	Attribute* attribute = hasAttribute(name);

	if (attribute == NULL) {
		return 0;
	}

	//Attribute exists and is an integer
	if (attribute->type == OSAL::TYPE_INT) {
		return attribute->intVal;
	}
		
	//TODO throw error because it exists and wasn't an int
	std::cout << "Tried to get incorrect type (int): " << name.c_str() << " type=" << ((int) attribute->type) << std::endl;
	return 0;

}

std::u16string oui::AttributeProfile::getString(std::string name) {

	Attribute* attribute = hasAttribute(name);

	if (attribute == NULL) {
		return u"";
	}

	//Attribute exists and is a string
	if (attribute->type == OSAL::TYPE_STRING) {
		return attribute->stringVal;
	}
		
	//TODO throw error because it exists and wasn't a string
	std::cout << "Tried to get incorrect type (string): " << name.c_str() << " type=" << ((int) attribute->type) << std::endl;
	return u"";

}

bool oui::AttributeProfile::getBool(std::string name) {

	Attribute* attribute = hasAttribute(name);

	if (attribute == NULL) {
		return false;
	}

	//Attribute exists and is a bool
	if (attribute->type == OSAL::TYPE_BOOL) {
		return attribute->boolVal;
	}
		
	//TODO throw error because it exists and wasn't a string
	std::cout << "Tried to get incorrect type (bool): " << name.c_str() << " type=" << ((int) attribute->type) << std::endl;
	return false;

}

double oui::AttributeProfile::getDouble(std::string name) {

	Attribute* attribute = hasAttribute(name);

	if (attribute == NULL) {
		return 0.0;
	}

	//Attribute exists and is a double
	if (attribute->type == OSAL::TYPE_DOUBLE) {
		return attribute->doubleVal;
	}
		
	//TODO throw error because it exists and wasn't a string
	std::cout << "Tried to get incorrect type (double): " << name.c_str() << " type=" << ((int) attribute->type) << std::endl;
	return 0.0;

}

void oui::AttributeProfile::removeAttribute(std::string name) {//TODO substitutions

	auto it = attributes.find(name);

	//Delete attribute if it exists
	if (it != attributes.end()) {
		attributes.erase(it);
	}

}

void oui::AttributeProfile::combineProfile(AttributeProfile* profile, bool overwrite) {

	auto i = profile->attributes.begin();

	//Loop through the new profile's attributes
	while (i != profile->attributes.end()) {

		//Set attribute if overwrite is true, or if it doesn't exist in this sheet
		if (overwrite || attributes.find(i->first) == attributes.end()) {
			setAttribute(i->first, i->second);
		}

		i++;
	}

}

bool oui::AttributeProfile::equals(AttributeProfile* other) {

	//Return false if non-existant
	if (other == NULL) {
		return false;
	}
	
	//Return false if they have different amount of attributes
	if (attributes.size() != other->attributes.size()) {
		return false;
	}

	auto thisIt = attributes.begin();
	auto otherIt = other->attributes.begin();

	//Loop through all attributes
	while (thisIt != attributes.end()) {

		//Return false if attributes are not equal
		if (!thisIt->second.equals(otherIt->second)) {
			return false;
		}

		thisIt++;
		otherIt++;
	}

	return true;
}

oui::AttributeProfile* oui::AttributeProfile::clone() {

	//Create new profile to store clone
	AttributeProfile* clone = new AttributeProfile(componentName);

	//Loop through and clone attributes
	auto it = attributes.begin();
	while (it != attributes.end()) {
		clone->setAttribute(it->first, it->second);

		it++;
	}

	return clone;

}