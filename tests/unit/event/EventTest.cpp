#include "TestUtil.h"

#include "event/OUI_Event2.h"

using namespace oui;

TEST(Event, it_can_be_created)
{
    Event2 event("test", 2);
    EXPECT_TRUE(event.compareType("test"));
    EXPECT_TRUE(event.compareType(2));
}

TEST(Event, it_can_be_created_without_a_hash)
{
    Event2 event("test");
    EXPECT_TRUE(event.compareType("test"));
    EXPECT_TRUE(event.compareType(Event2::createTypeHash("test")));
}

TEST(Event, it_can_prevent_default)
{
    Event2 event("test");
    EXPECT_FALSE(event.isDefaultPrevented());
    event.preventDefault();
    EXPECT_TRUE(event.isDefaultPrevented());
}

TEST(Event, it_can_compare_event_types)
{
    Event2 event("test");
    Event2 event2("test");
    EXPECT_TRUE(event.compareType(event2));
}

TEST(Event, it_can_compare_a_type_hash)
{
    Event2 event("test");
    EXPECT_TRUE(event.compareType(Event2::createTypeHash("test")));
}

TEST(Event, it_can_compare_a_type_string)
{
    Event2 event("test");
    EXPECT_TRUE(event.compareType("test"));
}