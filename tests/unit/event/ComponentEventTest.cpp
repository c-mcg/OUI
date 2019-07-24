#include "TestUtil.h"

#include "event/OUI_ComponentEvent.h"

using namespace oui;

const Component* NULL_COMP = NULL;

TEST(ComponentEvent, it_can_be_created)
{
    ComponentEvent event(NULL, "testClass", "test", false);
    EXPECT_STR_EQUAL(event.eventClass, "testClass");
    EXPECT_STR_EQUAL(event.type, "test");
    EXPECT_TRUE(event.compareType("test"));
    EXPECT_FALSE(event.bubbles);
    EXPECT_EQ(event.getTarget(),  NULL_COMP);
    EXPECT_EQ(event.originalTarget,  NULL_COMP);
}

TEST(ComponentEvent, it_can_be_created_without_a_hash)
{
    ComponentEvent event(NULL, "testClass", "test", false);
    EXPECT_STR_EQUAL(event.eventClass, "testClass");
    EXPECT_STR_EQUAL(event.type, "test");
    EXPECT_TRUE(event.compareType("test"));
    EXPECT_FALSE(event.bubbles);
    EXPECT_EQ(event.getTarget(),  NULL_COMP);
    EXPECT_EQ(event.originalTarget,  NULL_COMP);
}

TEST(ComponentEvent, it_can_stop_propagation)
{
    ComponentEvent event(NULL, "testClass", "test", false);
    EXPECT_FALSE(event.isPropagationStopped());
    event.stopPropagation();
    EXPECT_TRUE(event.isPropagationStopped());
}

TEST(ComponentEvent, it_can_set_a_target)
{
    ComponentEvent event(NULL, "testClass", "test", false);
    EXPECT_EQ(event.getTarget(),  NULL_COMP);
    event.setTarget((Component*) 4);
    EXPECT_EQ(event.getTarget(), (Component*) 4);
}
