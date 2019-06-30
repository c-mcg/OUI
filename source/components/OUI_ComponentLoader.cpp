
#include "osal/OSAL.h"
#include "components/OUI_ComponentLoader.h"
#include "components/OUI_DrawablePanel.h"
#include "components/OUI_Button.h"
#include "components/OUI_TextField.h"
#include "components/OUI_Menu.h"
#include "components/OUI_Panel.h"
#include "components/OUI_Label.h"
#include "components/OUI_ScrollPanel.h"

#include "util/OUI_StringUtil.h"

std::unordered_map<std::string, std::function<oui::Component*(std::string, std::string, std::vector<std::string>, std::vector<std::u16string>)>>  oui::ComponentLoader::tags;

bool oui::ComponentLoader::addTag(const std::string& tag, std::function < Component*(std::string, std::string, std::vector<std::string>, std::vector<std::u16string>)> createComponent) {
    auto it = tags.find(tag);
    if (it != tags.end()) {
        tags.erase(it);
    }
    tags.insert({tag, createComponent});
    return true;
}

oui::ComponentLoader::ComponentLoader() :
    components{std::vector<oui::Component*>()} {
}

bool oui::ComponentLoader::loadComponents(const std::u16string& path) {
    std::ifstream stream(convertUTF16toUTF8(path));
    if(!stream.good()) {//File doesnt exist
        std::cout << "Could not find file: " << convertUTF16toUTF8(path).c_str() << std::endl;
        return false;
    }
    std::string str =  std::string(std::istreambuf_iterator<char>(stream),
        std::istreambuf_iterator<char>());//Load file
    stream.close();

    oui::Container* currContainer = NULL;
    int state = 0;
    std::string tag = "";
    std::string attributeName = "";
    std::u16string attributeValue = u"";
    std::vector<std::string> attributeNames;
    std::vector<std::u16string> attributes;
    bool endTag = false;
    for(unsigned int i = 0; i < str.length(); i++) {
        char c = str.at(i);

        if(state == 0) {//looking for '<'
            endTag = false;
            if(c == '<') {
                state = 1;
            } else if(!isspace(c)) {
                //TODO error
            }
        } else if(state == 1) {//Looking for tag
            if(isalpha(c)) {
                tag = "";
                tag += c;
                attributeNames = std::vector<std::string>();
                attributes = std::vector<std::u16string>();
                state = 2;
            } else if(c == '/') {
                endTag = true;
            } else if(!isspace(c)) {
                //TODO error
            }
        } else if(state == 2) {//Building tag
            if(isalpha(c)) {
                tag += c;
            } else if(c == '"') {
                state = 4;
            } else if(c == '>' && endTag) {
                if(currContainer != NULL) {
                    currContainer = (Container*) currContainer->getParent();
                    state = 0;
                } else {
                    //TODO error
                }
            } else if(isspace(c)) {
                state = 3;
            } else {
                //TODO error
            }
        } else if(state == 3) {//Looking for attribute name
            if (isalpha(c)) {
                attributeName = c;
                state = 4;
            } else if(c == '>') {
                std::string name = "";
                std::string classes = "";
                for (int j = 0; j < attributeNames.size(); j++) {
                    if (attributeNames.at(j) == "name") {
                        name = convertUTF16toUTF8(attributes.at(j));
                    }
                    if (attributeNames.at(j) == "class") {
                        classes = convertUTF16toUTF8(attributes.at(j));
                    }
                }
                if (name == "") {
                    std::cout << "THE NAME IS EMPTY?!?!?!?" << std::endl;
                    //TODO error
                }
                Component* c = NULL;
                auto it = tags.find(tag);
                if (it != tags.end()) {
                    c = it->second(name, classes, attributeNames, attributes);
                }

                if(c != NULL) {
                    for (int j = 0; j < attributeNames.size(); j++) {
                        if (attributeNames.at(j) != "name" && attributeNames.at(j) != "class") {
                            c->parseAttribute(attributeNames.at(j), attributes.at(j));
                        }
                    }
                    if(currContainer != NULL) {
                        currContainer->addChild(c);
                    } else {
                        components.push_back(c);
                    }
                    if(c->isContainer()) {
                        currContainer = (Container*) c;
                    }
                }
                state = 0;
            } else if (isspace(c)) {
                //TODO error
            }
        } else if(state == 4) {//Building attribute name
            if (isalpha(c) || c == '-') {
                attributeName += c;
            } else if (c == '=') {
                state = 5;
            } else if (isspace(c)) {
                //TODO error
            }
        } else if(state == 5) {//Found '=' looking for '"'
            if (c == '"') {
                attributeValue = u"";
                state = 6;
            } else if (isspace(c)) {
                //TODO error
            }
        } else if(state == 6) {//building attribute value
            if (c == '"') {
                attributeNames.push_back(attributeName);
                attributes.push_back(attributeValue);
                state = 3;
            } else {
                attributeValue += (char16_t) c;
            }
        } else if(state == 7) {
            
        }

    }

    return false;
}


oui::Panel* oui::ComponentLoader::toPanel() {
    Panel* panel = new Panel("loadedpanel", "");
    panel->parseAttribute("size", u"100 100 0 0");
    panel->addOSALStyle(attributes);
    for(unsigned int i = 0; i < components.size(); i++) {
        panel->addChild(components.at(i));
    }
    return panel;
}