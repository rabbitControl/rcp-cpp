#include <catch2/catch_test_macros.hpp>

#include <rcp/0.1.0/ParameterDefinitions.h>
#include <rcp/0.1.0/GroupParameter.h>


namespace rcp {
namespace v010 {


TEST_CASE( "v010 Group Parameter" )
{

    auto group = v010::GroupParameter::create(1);
    REQUIRE(group);

    group->setLabel("group");


    auto group1 = GroupParameter::create(2);
    REQUIRE(group1);

    group1->setLabel("group 1");


    auto param1 = StringParameter::create(10);
    REQUIRE(param1);

    param1->setLabel("param 1");
    param1->setValue("uno");

    auto param2 = StringParameter::create(11);
    REQUIRE(param2);

    param2->setLabel("param 2");
    param2->setValue("dos");



    group->addChild(param1);


    if (auto p = param1->getParent().lock())
    {
        REQUIRE(p == group);
    }


    group1->addChild(param1);

    if (auto p = param1->getParent().lock())
    {
        REQUIRE(p == group1);
    }


    group->removeChild(param1);
    REQUIRE(param1->getParent().lock());

    group1->removeChild(param1);
    REQUIRE(!param1->getParent().lock());

}


}
}
