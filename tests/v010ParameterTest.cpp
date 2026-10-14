#include <catch2/catch_test_macros.hpp>

#include <rcp/0.1.0/ParameterDefinitions.h>

#include "0.1.0/Helper.h"

namespace rcp {
namespace v010 {

TEST_CASE( "v010 String Parameter update" )
{
    auto param = StringParameter::create(10);

    REQUIRE(param);

    param->setLabel("string label");
    param->setValue("Test value");
    param->typeDefinition().setRegex("test");
    param->typeDefinition().setDefaultValue("default TEST");

    bool value_changed_cb_triggered = false;
    param->addValueUpdatedCb(param.get(),
                             [&value_changed_cb_triggered](const std::string& value)
    {
        std::cout << "string value changed:" << value << std::endl;
        value_changed_cb_triggered = true;
    });

    auto param_2 = StringParameter::create(10);
    param_2->setLabel("string label 2");
    param_2->setValue("Test value 1");
    param_2->setOrder(10);
    param_2->setReadonly(true);
    param_2->setUserid("user id");

    param_2->typeDefinition().setRegex("test 1");
    param_2->typeDefinition().setDefaultValue("default TEST 1");


    param->update(param_2);

    REQUIRE(value_changed_cb_triggered);

    REQUIRE(param->getLabel() == param_2->getLabel());
    REQUIRE(param->getValue() == param_2->getValue());
    REQUIRE(param->getOrder() == param_2->getOrder());
    REQUIRE(param->getReadonly() == param_2->getReadonly());
    REQUIRE(param->getUserid() == param_2->getUserid());

    REQUIRE(param->typeDefinition().getRegex() == param_2->typeDefinition().getRegex());
    REQUIRE(param->typeDefinition().getDefaultValue() == param_2->typeDefinition().getDefaultValue());
}



TEST_CASE("Bang 010", "0.1.0")
{
    auto bang = BangParameter::create(2);

    REQUIRE(bang);

    bool banged = false;

    bang->setLabel("Bang");
    bang->setBangCb([&banged]
    {
        banged = true;
    });


    auto bang_2 = BangParameter::create(2);

    bang->update(bang_2);


    REQUIRE(banged);
}

TEST_CASE("Bang Parameter update 010", "0.1.0")
{
    auto bang = BangParameter::create(2);

    REQUIRE(bang);

    bool banged = false;

    bang->setLabel("Bang");
    bang->setBangCb([&banged]
    {
        banged = true;
    });


    auto bang_2 = BangParameter::create(2);
    bang_2->setLabel("Bang 2");
    bang_2->setReadonly(true);

    bang->update(bang_2);


    REQUIRE(!banged);

    REQUIRE(bang->getLabel() == bang_2->getLabel());
    REQUIRE(bang->getReadonly() == bang_2->getReadonly());
}


TEST_CASE("Boolean 010", "0.1.0")
{
    auto bool_param = BooleanParameter::create(10);

    REQUIRE(bool_param);

    bool_param->setValue(true);
    bool_param->typeDefinition().setDefaultValue(true);
}



TEST_CASE("Enum 010", "0.1.0")
{
    auto param = EnumParameter::create(2);
    param->typeDefinition().addOption("test 1");
    param->typeDefinition().addOption("test 2");
    param->typeDefinition().addOption("test 3");
    param->setValue("test 1");

    write(param, true);

    auto pp = reparse<EnumParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->getValue() == param->getValue());
    REQUIRE(pp->typeDefinition().getOptions() == param->typeDefinition().getOptions());
}


}
}

