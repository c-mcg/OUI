#include "component/impl/scrollpanel/OUI_ScrollPanel.h"

#include "window/OUI_window.h"
#include "component/impl/container/OUI_Container.h"
#include "component/impl/button/OUI_Button.h"
#include "event/OUI_ScrollEvent.h"
#include "util/OUI_StringUtil.h"
#include "component/attribute/OUI_AttributeNames.h"

using namespace oui::AttributeNames;

oui::ScrollPanel::~ScrollPanel() {

}

oui::ScrollPanel::ScrollPanel(const std::string& name, const std::string& classes, EventDispatcher* eventdispatcher, ScrollPanelAttributeManager* attributeManager) :
	childWidth{0}, childHeight{0}, clickStartX{0}, clickStartY{0}, resetX{0}, resetY{0},
	scrollPosX{0}, scrollPosY{0}, scrollingX{false}, scrollingY{false}, childrenChanged{false},
    Container("scrollpanel", name, classes, true, eventDispatcher, attributeManager)
{
    createScrollBar(true);
    createScrollBar(false);
}

void oui::ScrollPanel::createScrollBar(bool vertical) {

    Container* scrollBar = new Container("scrollbar", std::string(vertical ? "vertical" : "horizontal") + "ScrollBar", "scrollBar");
    scrollBar->setAttribute(PERMANENT, true);

    scrollBar->setAttribute(WIDTH_PERCENT, vertical ? 0 : 100);
    scrollBar->setAttribute(HEIGHT_PERCENT, vertical ? 100 : 0);
    scrollBar->setAttribute(WIDTH_OFFSET, vertical ? SCROLLBAR_SIZE : -SCROLLBAR_SIZE);
    scrollBar->setAttribute(HEIGHT_OFFSET, vertical ? -SCROLLBAR_SIZE : SCROLLBAR_SIZE);


    scrollBar->setAttribute(X_PERCENT, vertical ? 100 : 0);
    scrollBar->setAttribute(Y_PERCENT, vertical ? 0 : 100);
    scrollBar->setAttribute(X_OFFSET, vertical ? -SCROLLBAR_SIZE : 0);
    scrollBar->setAttribute(Y_OFFSET, vertical ? 0 : -SCROLLBAR_SIZE);

    scrollBar->setAttribute(BACKGROUND_COLOR_1, Color(230, 230, 230, 255));
    scrollBar->setAttribute(BACKGROUND_COLOR_2, Color(230, 230, 230, 255));
    scrollBar->setAttribute(Z_INDEX, 10);

    scrollBar->addEventListener("scroll", [this, vertical](ComponentEvent* compEvent) {
        ScrollEvent* event = static_cast<ScrollEvent*>(compEvent);
        if (vertical) {
            scrollY(event->scrollDistance * SCROLL_WHEEL_SPEED);
        } else {
            scrollX(event->scrollDistance * SCROLL_WHEEL_SPEED);
        }
    });

    Button* button1 = new Button(vertical ? "upBtn" : "leftBtn", "scrollbarButton");
    button1->setAttribute(WIDTH_OFFSET, SCROLLBAR_SIZE);
    button1->setAttribute(HEIGHT_OFFSET, SCROLLBAR_SIZE);
    button1->parseAttribute(LOCATION, u"0 0 0 0");

    button1->setAttribute(BACKGROUND_COLOR_1, Color::NONE);
    button1->setAttribute(BACKGROUND_COLOR_2, Color::NONE);
    button1->setAttribute(BORDER_STYLE, u"none");

    button1->setAttribute(IMAGE, vertical ? "up-arrow.png" : "left-arrow.png");
    button1->setAttribute(BACKGROUND_COLOR_1, Color(190, 190, 190, 255), u"hover");
    button1->setAttribute(BACKGROUND_COLOR_2, Color(190, 190, 190, 255), u"hover");

    button1->addEventListener("click", [this, vertical](ComponentEvent* e) {
        if (vertical) {
            scrollY(SCROLL_BUTTON_SPEED);
        } else {
            scrollX(SCROLL_BUTTON_SPEED);
        }
    });

    scrollBar->addChild(button1);

    Button* button2 = new Button(vertical ? "downBtn" : "rightBtn", "scrollbarButton");
    button2->setAttribute(WIDTH_OFFSET, SCROLLBAR_SIZE);
    button2->setAttribute(HEIGHT_OFFSET, SCROLLBAR_SIZE);

    button2->setAttribute(X_PERCENT, vertical ? 0 : 100);
    button2->setAttribute(Y_PERCENT, vertical ? 100 : 0);
    button2->setAttribute(X_OFFSET, vertical ? 0 : -SCROLLBAR_SIZE);
    button2->setAttribute(Y_OFFSET, vertical ? -SCROLLBAR_SIZE : 0);

    button2->setAttribute(BACKGROUND_COLOR_1, Color::NONE);
    button2->setAttribute(BACKGROUND_COLOR_2, Color::NONE);
    button2->setAttribute(BORDER_STYLE, u"none");

    button2->setAttribute(IMAGE, vertical ? "down-arrow.png" : "right-arrow.png");
    button2->setAttribute(BACKGROUND_COLOR_1, Color(190, 190, 190, 255), u"hover");
    button2->setAttribute(BACKGROUND_COLOR_2, Color(190, 190, 190, 255), u"hover");

    button2->addEventListener("click", [this, vertical](ComponentEvent* e) {
        if (vertical) {
            scrollY(-SCROLL_BUTTON_SPEED);
        } else {
            scrollX(-SCROLL_BUTTON_SPEED);
        }
    });

    scrollBar->addChild(button2);

    Button* bar = new Button("bar", "scrollbarBar");
    bar->setAttribute(WIDTH_PERCENT, vertical ? 100 : 0);
    bar->setAttribute(HEIGHT_PERCENT, vertical ?0 : 100);
    bar->setAttribute(WIDTH_OFFSET, vertical ? 0 : -SCROLLBAR_SIZE * 2);
    bar->setAttribute(HEIGHT_OFFSET, vertical ? -SCROLLBAR_SIZE * 2 : 0);

    bar->setAttribute(X_PERCENT, 0);
    bar->setAttribute(Y_PERCENT, 0);
    bar->setAttribute(X_OFFSET, vertical ? 0 : SCROLLBAR_SIZE);
    bar->setAttribute(Y_OFFSET, vertical ? SCROLLBAR_SIZE : 0);
    
    bar->setAttribute(BACKGROUND_COLOR_1, Color(180, 180, 180, 255));
    bar->setAttribute(BACKGROUND_COLOR_2, Color(180, 180, 180, 255));
    bar->setAttribute(BACKGROUND_COLOR_1, Color(160, 160, 160, 255), u"hover");
    bar->setAttribute(BACKGROUND_COLOR_2, Color(160, 160, 160, 255), u"hover");
    bar->setAttribute(BORDER_STYLE, u"none");

    bar->addEventListener("mousedown", [this, vertical](ComponentEvent* compEvent) {
        MouseEvent* event = static_cast<MouseEvent*>(compEvent);
        auto comp = event->getTarget();
        scrollingY = vertical;
        scrollingX = !vertical;
        if (vertical) {
            clickStartY = event->localY;
        } else {
            clickStartX = event->localX;
        }
        comp->flagGraphicsUpdate();
    });

    bar->addEventListener("mousedown", [this, vertical](ComponentEvent* e) {
        scrollingX = false;
        scrollingY = false;
    });

    scrollBar->addChild(bar);

    addScrollBar(scrollBar);

    addEventListener("scroll", std::bind(&ScrollPanel::onScroll, this, std::placeholders::_1));
}

void oui::ScrollPanel::getChildSize(int* w, int* h) {
    int boundsX = getWidth();
    int boundsY = getHeight();
    for (unsigned int i = 0; i < children.size(); i++) {
        Component* c = children.at(i);
        int w = c->getRelativeX() + c->getWidth();
        int h = c->getRelativeY() + c->getHeight();
        if (w > boundsX) {
            boundsX = w;
        }
        if (h > boundsY) {
            boundsY = h;
        }
    }
    *w = boundsX;
    *h = boundsY;
}

void oui::ScrollPanel::onScroll(ComponentEvent* e) {
    ScrollEvent* scrollEvt = static_cast<ScrollEvent*>(e);
    scrollY(scrollEvt->scrollDistance * SCROLL_WHEEL_SPEED);
}

bool oui::ScrollPanel::addScrollBar(Container* scrollbar) {
    return Container::addChild(scrollbar);
}

bool oui::ScrollPanel::addChild(Component* child) {
    if (Container::addChild(child)) {
        //TODO maybe this resize could happen when children are redrawn
        //that way it won't resize when adding 8 children
        //an addChildren method would just be a band-aid
        childrenChanged = true;
        return true;
    }
    return false;
}

oui::Component* oui::ScrollPanel::removeChild(int index, bool shouldDelete) {
    Component* c = Container::removeChild(index, shouldDelete);

    //Resize if one got removed
    if (c != NULL) {
        //TODO maybe this resize could happen when children are redrawn
        //that way it won't resize when adding 8 children
        //a removeChildren method would just be a band-aid
        childrenChanged = true;
        return c;
    }

    return NULL;
}

void oui::ScrollPanel::removeAllChildren(bool shouldDelete) {
    Container::removeAllChildren(shouldDelete);
    childrenChanged = true;
}


int oui::ScrollPanel::process() {

    if (childrenChanged) {
        handleResize();
        childrenChanged = false;
    }
    
    return 0;
}

void oui::ScrollPanel::redraw() {
    Container::redraw();
    if (scrollingX || scrollingY || resetX || resetY) {//TODO out of window scrolling (getglobalmousedown)
        //TODO move this to process (calculating bar position)

        if (scrollingY || resetY) {//TODO resizing is kinda glitchy sometimes (visibility check here is a cheaphax fix)
            Component* c = getChildCont("verticalScrollBar")->getChild("bar");
            if (!c->getParent()->getAttributeManager()->isVisible()) {
                scrollPosY = 0;
                resetY = true;
            }

            int spaceSize = getHeight() - SCROLLBAR_SIZE * 2;
            int pageDifference = childHeight - getHeight();
            int barDifference = spaceSize - c->getHeight();

            int barY = c->getWindow()->getMouseY() - c->getParent()->getRelativeY() - clickStartY;
            if (resetY && pageDifference != 0 && barDifference != 0) {
                barY = (int) (scrollPosY / (pageDifference / (float) barDifference) + SCROLLBAR_SIZE);
            }
            if (barY < SCROLLBAR_SIZE ) {
                barY = SCROLLBAR_SIZE;
            }
            if (barY + c->getHeight() > c->getParent()->getHeight() - SCROLLBAR_SIZE) {
                barY = c->getParent()->getHeight() - SCROLLBAR_SIZE - c->getHeight();
            }
            c->parseAttribute(LOCATION, u"0 0 0 " + intToString(barY));

            int scrollPosY = (int) (barDifference == 0 ? 0 : (barY - SCROLLBAR_SIZE) * (pageDifference / (float) barDifference));
            for (unsigned int i = 0; i < children.size(); i++) {
                Component* target = children.at(i);
                if (!target->compareTag("scrollbar")) {
                    target->setScrollOffsetY(scrollPosY);
                }
            }

            if (!resetY) {
                this->scrollPosY = scrollPosY;
            }
        }
        if (scrollingX || resetX) {
            Component* c = getChildCont("horizontalScrollBar")->getChild("bar");
            if (!c->getParent()->getAttributeManager()->isVisible()) {
                scrollPosX = 0;
                resetX = true;
            }

            int spaceSize = getWidth() - SCROLLBAR_SIZE * 2;
            int pageDifference = childWidth - getWidth();
            int barDifference = spaceSize - c->getWidth();

            int barX = c->getWindow()->getMouseX() - c->getParent()->getRelativeX() - clickStartX;
            if (resetX && pageDifference != 0 && barDifference != 0) {
                barX = (int) (scrollPosX / (pageDifference / (float) barDifference) + SCROLLBAR_SIZE);
            }
            if (barX < SCROLLBAR_SIZE) {
                barX = SCROLLBAR_SIZE;
            }
            if (barX + c->getWidth() > c->getParent()->getWidth() - SCROLLBAR_SIZE) {
                barX = c->getParent()->getWidth() - SCROLLBAR_SIZE - c->getWidth();
            }
            c->parseAttribute(LOCATION, u"0 0 " + intToString(barX).append(u" 0"));

            int scrollPosX = (int) (barDifference == 0 ? 0 : (barX - SCROLLBAR_SIZE) * (pageDifference / (float) barDifference));
            for (unsigned int i = 0; i < children.size(); i++) {
                Component* target = children.at(i);
                if (!target->compareTag("scrollbar")) {
                    target->setScrollOffsetX(scrollPosX);
                }
            }

            if (!resetX) {
                this->scrollPosX = scrollPosX;
            }
        }
        
        if (window != NULL && !window->isMouseDown()) {
            scrollingX = false;
            scrollingY = false;
        }
        if (resetX || resetY) {
            resetX = false;
            resetY = false;
        }
    }
}

void oui::ScrollPanel::handleResize() {
    if (getWidth() == 0 || getHeight() == 0) return;

    //TODO the reason they scroll during resize is because the bar positions should be based on a fixed percentage during resizing (implementable in this function)
    getChildSize(&childWidth, &childHeight);

    //TODO resize screen so children don't hide behind scrollbars

    //Horizontal scrollbar resize
    Container* hSB = static_cast<Container*>(getChild("horizontalScrollBar"));
    Container* vSB = static_cast<Container*>(getChild("verticalScrollBar"));

    float hRatio = childWidth / (float) getWidth();
    float vRatio = childHeight / (float) getHeight();

    hSB->setAttribute(VISIBLE, childWidth > getWidth());
    vSB->setAttribute(VISIBLE, childHeight > getHeight());

    //Resize to avoid overlap
    int width, height;
    width = vSB->getAttributeManager()->isVisible() ? -SCROLLBAR_SIZE : 0;
    height = SCROLLBAR_SIZE;
    hSB->setAttribute(WIDTH_OFFSET, width);
    hSB->setAttribute(HEIGHT_OFFSET, height);

    width = SCROLLBAR_SIZE;
    height = hSB->getAttributeManager()->isVisible() ? -SCROLLBAR_SIZE : 0;
    vSB->setAttribute(WIDTH_OFFSET, width);
    vSB->setAttribute(HEIGHT_OFFSET, height);

    //Horizontal scrollbar bar resize
    int spaceSize = (getWidth() - SCROLLBAR_SIZE * 2);
    int barSize = (int) (spaceSize / hRatio);
    Component* bar = hSB->getChild("bar");
    bar->setAttribute(WIDTH_OFFSET, barSize);
    bar->setAttribute(HEIGHT_OFFSET, 0);
    resetX = true;

    //Vertical scrollbar bar resize
    spaceSize = (getHeight() - SCROLLBAR_SIZE * 2);
    barSize = (int) (spaceSize / vRatio);
    bar = vSB->getChild("bar");
    bar->setAttribute(WIDTH_OFFSET, 0);
    bar->setAttribute(HEIGHT_OFFSET, barSize);
    resetY = true;
}

void oui::ScrollPanel::scrollX(int speed) {
    scrollPosX -= speed;
    if (scrollPosX < 0) {
        scrollPosX = 0;
    }
    if (scrollPosX > childWidth - getWidth()) {
        scrollPosX = childWidth - getWidth();
    }
    resetX = true;
    flagGraphicsUpdate();
}

void oui::ScrollPanel::scrollY(int speed) {
    scrollPosY -= speed;
    if (scrollPosY < 0) {
        scrollPosY = 0;
    }
    if (scrollPosY > childHeight - getHeight()) {
        scrollPosY = childHeight - getHeight();
    }
    resetY = true;
    flagGraphicsUpdate();
}

oui::ScrollPanelAttributeManager* oui::ScrollPanel::getAttributeManager() {
    return static_cast<ScrollPanelAttributeManager*>(attributeManager);
}