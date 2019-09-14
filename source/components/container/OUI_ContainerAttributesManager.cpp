#include "components/container/OUI_ContainerAttributeManager.h"

#include "components/container/OUI_Container.h"

oui::ContainerAttributeManager::~ContainerAttributeManager() {
    if (this->styleSheet != NULL) {
        delete this->styleSheet;
        this->styleSheet = NULL;
    }
}

oui::ContainerAttributeManager::ContainerAttributeManager():
    styleSheet{NULL},
    ComponentAttributeManager()
{

}


void oui::ContainerAttributeManager::refreshProfile() {
    Container* container = static_cast<Container*>(component);

    int width = container->getWidth();
    int height = container->getHeight();
    Graphics* graphics = container->getGraphics();

    ComponentAttributeManager::refreshProfile();
    if (graphics != NULL) {
        if (width != graphics->getWidth() || height != graphics->getHeight()) {
            container->flagGraphicsUpdateAll();
        }
    }
}

void oui::ContainerAttributeManager::deriveAttributesForComponent(StyleSheet* styleSheet) {
    Container* container = static_cast<Container*>(component);

     if (styleSheet == NULL) {
        styleSheet = container->getAllStyleSheets();

        if (styleSheet == NULL) {
            return;
        }
    }

    if (styleSheet != this->styleSheet) {
        StyleSheet* clone = styleSheet->clone(); //Not inline for debugging
        setStyleSheet(clone);
    }

    
    for(unsigned int i = 0; i < container->getNumChildren(); i++) {
        container->getChild(i)->deriveAttributesForComponent(styleSheet);
    }

    ComponentAttributeManager::deriveAttributesForComponent(styleSheet);
}

oui::StyleSheet* oui::ContainerAttributeManager::getStyleSheet() {
    return styleSheet;
}

void oui::ContainerAttributeManager::addStyleSheet(StyleSheet* sheet) {
    if(styleSheet == NULL) {
        this->styleSheet = sheet;
    } else {
        this->styleSheet->combineStyleSheet(sheet, true);
    }
    deriveAttributesForComponent(this->styleSheet);
    if (getBool("visible")) {
        refreshProfile();
    }
}

oui::StyleSheet* oui::ContainerAttributeManager::getAllStyleSheets() {

    StyleSheet* sheet = new StyleSheet();

    Container* parent = component->getParent();
    if(parent != NULL) {

        StyleSheet* parentSheet = parent->getAllStyleSheets();

        if (parentSheet != NULL) {
            sheet->combineStyleSheet(parentSheet);
        }

    }

    //Make sure something exists before we add this styleSheet
    //This makes it so we only need to check for empty sheet later, as opposed to looking for equality to styleSheet
    if (!sheet->isEmpty()) {

        //Add this style sheet
        if (this->styleSheet != NULL && !this->styleSheet->isEmpty()) {
            sheet->combineStyleSheet(this->styleSheet, false);
        }

    }

    //Delete and replace current style sheet
    //Above check lets us assume if it's not empty, then it isn't equal to `this->styleSheet`
    if (!sheet->isEmpty()) {

        //Sets the current style sheet, deletes the old one if it exists
        setStyleSheet(sheet);

        return sheet;
    }

    //We didn't replace the styleSheet so we'll delete this
    delete sheet;
    sheet = NULL;

    //Return the current styleSheet
    return styleSheet;
}

void oui::ContainerAttributeManager::updateAttributeVariable(const std::string& attributeName, Attribute value) {
    ComponentAttributeManager::updateAttributeVariable(attributeName, value);
}

void oui::ContainerAttributeManager::setStyleSheet(StyleSheet* sheet) {

    if (this->styleSheet != sheet && this->styleSheet != NULL) {
        delete this->styleSheet;
    }

    this->styleSheet = sheet;
}
