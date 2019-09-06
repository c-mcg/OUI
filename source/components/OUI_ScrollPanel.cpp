#include "components/OUI_ScrollPanel.h"

#include "OUI_Window.h"
#include "components/OUI_Container.h"
#include "components/OUI_Button.h"
#include "event/OUI_ScrollEvent.h"
#include "util/OUI_StringUtil.h"

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
    scrollBar->setAttribute("permanent", true);

    scrollBar->setAttribute("width-percent", vertical ? 0 : 100);
    scrollBar->setAttribute("height-percent", vertical ? 100 : 0);
    scrollBar->setAttribute("width-offset", vertical ? SCROLLBAR_SIZE : -SCROLLBAR_SIZE);
    scrollBar->setAttribute("height-offset", vertical ? -SCROLLBAR_SIZE : SCROLLBAR_SIZE);


    scrollBar->setAttribute("x-percent", vertical ? 100 : 0);
    scrollBar->setAttribute("y-percent", vertical ? 0 : 100);
    scrollBar->setAttribute("x-offset", vertical ? -SCROLLBAR_SIZE : 0);
    scrollBar->setAttribute("y-offset", vertical ? 0 : -SCROLLBAR_SIZE);

    scrollBar->parseAttribute("bg-color", u"230 230 230 255");
    scrollBar->setAttribute("z-index", 10);

    scrollBar->addEventListener("scroll", [this, vertical](ComponentEvent* compEvent) {
        ScrollEvent* event = static_cast<ScrollEvent*>(compEvent);
        if (vertical) {
            scrollY(event->scrollDistance * SCROLL_WHEEL_SPEED);
        } else {
            scrollX(event->scrollDistance * SCROLL_WHEEL_SPEED);
        }
    });

    Button* button1 = new Button(vertical ? "upBtn" : "leftBtn", "scrollbarButton");
    button1->setAttribute("width-offset", SCROLLBAR_SIZE);
    button1->setAttribute("height-offset", SCROLLBAR_SIZE);
    button1->parseAttribute("location", u"0 0 0 0");

    button1->parseAttribute("bg-color", u"0 0 0 0");
    button1->setAttribute("border-style", u"none");

    button1->setAttribute("image", vertical ? "up-arrow.png" : "left-arrow.png");
    button1->parseAttribute("bg-color", u"190 190 190 255", u"hover");

    button1->addEventListener("click", [this, vertical](ComponentEvent* e) {
        if (vertical) {
            scrollY(SCROLL_BUTTON_SPEED);
        } else {
            scrollX(SCROLL_BUTTON_SPEED);
        }
    });

    scrollBar->addChild(button1);

    Button* button2 = new Button(vertical ? "downBtn" : "rightBtn", "scrollbarButton");
    button2->setAttribute("width-offset", SCROLLBAR_SIZE);
    button2->setAttribute("height-offset", SCROLLBAR_SIZE);

    button2->setAttribute("x-percent", vertical ? 0 : 100);
    button2->setAttribute("y-percent", vertical ? 100 : 0);
    button2->setAttribute("x-offset", vertical ? 0 : -SCROLLBAR_SIZE);
    button2->setAttribute("y-offset", vertical ? -SCROLLBAR_SIZE : 0);

    button2->parseAttribute("bg-color", u"0 0 0 0");
    button2->setAttribute("border-style", u"none");

    button2->setAttribute("image", vertical ? "down-arrow.png" : "right-arrow.png");
    button2->parseAttribute("bg-color", u"190 190 190 255", u"hover");

    button2->addEventListener("click", [this, vertical](ComponentEvent* e) {
        if (vertical) {
            scrollY(-SCROLL_BUTTON_SPEED);
        } else {
            scrollX(-SCROLL_BUTTON_SPEED);
        }
    });

    scrollBar->addChild(button2);

    Button* bar = new Button("bar", "scrollbarBar");
    bar->setAttribute("width-percent", vertical ? 100 : 0);
    bar->setAttribute("height-percent", vertical ?0 : 100);
    bar->setAttribute("width-offset", vertical ? 0 : -SCROLLBAR_SIZE * 2);
    bar->setAttribute("height-offset", vertical ? -SCROLLBAR_SIZE * 2 : 0);

    bar->setAttribute("x-percent", 0);
    bar->setAttribute("y-percent", 0);
    bar->setAttribute("x-offset", vertical ? 0 : SCROLLBAR_SIZE);
    bar->setAttribute("y-offset", vertical ? SCROLLBAR_SIZE : 0);
    
    bar->parseAttribute("bg-color", u"180 180 180 255");
    bar->parseAttribute("bg-color", u"160 160 160 255", u"hover");
    bar->setAttribute("border-style", u"none");

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
        int w = c->getX() + c->getWidth();
        int h = c->getY() + c->getHeight();
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
            if (!c->getParent()->getBool("visible")) {
                scrollPosY = 0;
                resetY = true;
            }

            int spaceSize = getHeight() - SCROLLBAR_SIZE * 2;
            int pageDifference = childHeight - getHeight();
            int barDifference = spaceSize - c->getHeight();

            int barY = c->getWindow()->getMouseY() - c->getParent()->getY() - clickStartY;
            if (resetY && pageDifference != 0 && barDifference != 0) {
                barY = (int) (scrollPosY / (pageDifference / (float) barDifference) + SCROLLBAR_SIZE);
            }
            if (barY < SCROLLBAR_SIZE ) {
                barY = SCROLLBAR_SIZE;
            }
            if (barY + c->getHeight() > c->getParent()->getHeight() - SCROLLBAR_SIZE) {
                barY = c->getParent()->getHeight() - SCROLLBAR_SIZE - c->getHeight();
            }
            c->parseAttribute("location", u"0 0 0 " + intToString(barY));

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
            if (!c->getParent()->getBool("visible")) {
                scrollPosX = 0;
                resetX = true;
            }

            int spaceSize = getWidth() - SCROLLBAR_SIZE * 2;
            int pageDifference = childWidth - getWidth();
            int barDifference = spaceSize - c->getWidth();

            int barX = c->getWindow()->getMouseX() - c->getParent()->getX() - clickStartX;
            if (resetX && pageDifference != 0 && barDifference != 0) {
                barX = (int) (scrollPosX / (pageDifference / (float) barDifference) + SCROLLBAR_SIZE);
            }
            if (barX < SCROLLBAR_SIZE) {
                barX = SCROLLBAR_SIZE;
            }
            if (barX + c->getWidth() > c->getParent()->getWidth() - SCROLLBAR_SIZE) {
                barX = c->getParent()->getWidth() - SCROLLBAR_SIZE - c->getWidth();
            }
            c->parseAttribute("location", u"0 0 " + intToString(barX).append(u" 0"));

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

    hSB->setAttribute("visible", childWidth > getWidth());
    vSB->setAttribute("visible", childHeight > getHeight());

    //Resize to avoid overlap
    int width, height;
    width = vSB->getBool("visible") ? -SCROLLBAR_SIZE : 0;
    height = SCROLLBAR_SIZE;
    hSB->setAttribute("width-offset", width);
    hSB->setAttribute("height-offset", height);

    width = SCROLLBAR_SIZE;
    height = hSB->getBool("visible") ? -SCROLLBAR_SIZE : 0;
    vSB->setAttribute("width-offset", width);
    vSB->setAttribute("height-offset", height);

    //Horizontal scrollbar bar resize
    int spaceSize = (getWidth() - SCROLLBAR_SIZE * 2);
    int barSize = (int) (spaceSize / hRatio);
    Component* bar = hSB->getChild("bar");
    bar->setAttribute("width-offset", barSize);
    bar->setAttribute("height-offset", 0);
    resetX = true;

    //Vertical scrollbar bar resize
    spaceSize = (getHeight() - SCROLLBAR_SIZE * 2);
    barSize = (int) (spaceSize / vRatio);
    bar = vSB->getChild("bar");
    bar->setAttribute("width-offset", 0);
    bar->setAttribute("height-offset", barSize);
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