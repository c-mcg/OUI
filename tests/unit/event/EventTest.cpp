#include "TestUtil.h"

#include "event/OUI_Event.h"

using namespace oui;

TEST(Event, it_can_be_created)
{
    Event event("testClass", "test", 2);
    EXPECT_STR_EQUAL(event.eventClass, "testClass");
    EXPECT_STR_EQUAL(event.type, "test");
    EXPECT_TRUE(event.compareType("test"));
    EXPECT_TRUE(event.compareType(2));
}

TEST(Event, it_can_be_created_without_a_hash)
{
    Event event("testClass", "test");
    EXPECT_STR_EQUAL(event.eventClass, "testClass");
    EXPECT_STR_EQUAL(event.type, "test");
    EXPECT_TRUE(event.compareType("test"));
    EXPECT_TRUE(event.compareType(Event::createTypeHash("test")));
}

TEST(Event, it_can_prevent_default)
{
    Event event("testClass", "test");
    EXPECT_FALSE(event.isDefaultPrevented());
    event.preventDefault();
    EXPECT_TRUE(event.isDefaultPrevented());
}

TEST(Event, it_can_compare_event_types)
{
    Event event("testClass", "test");
    Event event2("testClass", "test");
    EXPECT_TRUE(event.compareType(event2));
}

TEST(Event, it_can_compare_a_type_hash)
{
    Event event("testClass", "test");
    EXPECT_TRUE(event.compareType(Event::createTypeHash("test")));
}

TEST(Event, it_can_compare_a_type_string)
{
    Event event("testClass", "test");
    EXPECT_TRUE(event.compareType("test"));
}