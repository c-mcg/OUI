#include "TestUtil.h"

#include "parsers/xml/OUI_XMLObject.h"

TEST(XMLObject, itCanBeCreated)
{
    const std::string tag = "test";
    std::unordered_map<std::string, std::u16string> attributes;
    attributes.insert({"attr", u"1"});

    oui::XMLObject obj = oui::XMLObject(tag, attributes); 

    EXPECT_STR_EQUAL(obj.tag, tag);
    EXPECT_STR16_EQUAL(obj.getAttribute("attr"), u"1");
}