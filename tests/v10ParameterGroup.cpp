#include <catch2/catch_test_macros.hpp>

#include <rcp/1.0/ParameterDefinitions.h>
#include <rcp/1.0/GroupParameter.h>

#include "1.0/Helper.h"



namespace rcp {
namespace v10 {


TEST_CASE( "v10 Group Parameter" )
{

    auto group = GroupParameter::create(1);
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


TEST_CASE( "Group Parameter", "1.0" )
{

    auto param = GroupParameter::create(1);
    REQUIRE(param);

    param->setLabel("group");

    auto pp = reparse<GroupParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->getLabel() == param->getLabel());
}


TEST_CASE("Parenting", "1.0")
{
    auto group = GroupParameter::create(1);
    REQUIRE(group);

    auto param = StringParameter::create(2);
    REQUIRE(param);

    group->addChild(param);

    auto pp = reparse<StringParameter>(param, group);
    REQUIRE(pp);
    REQUIRE(pp->getParentId() == param->getParentId());

    param->removeFromParent();
    REQUIRE(pp->getParentId() != param->getParentId());
}

}
}
