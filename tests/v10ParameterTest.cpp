#include <catch2/catch_test_macros.hpp>

#include <rcp/StringStreamWriter.h>
#include <rcp/1.0/ParameterWriter.h>
#include <rcp/1.0/ParameterDefinitions.h>

#include "1.0/Helper.h"

namespace rcp {
namespace v10 {

TEST_CASE("String Parameter update", "1.0")
{
    auto param = StringParameter::create(10);

    REQUIRE(param);

    param->setLabel("string label");
    param->setValue("Test value");
    param->typeDefinition().setRegex("test");
    param->typeDefinition().setDefaultValue("default TEST");

    bool value_changed_cb_triggered = false;
    param->addValueUpdatedCb(param.get(), [&value_changed_cb_triggered](const std::string& value)
    {
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



TEST_CASE("Bang 10", "1.0")
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

TEST_CASE("Bang Parameter update 10", "1.0")
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



TEST_CASE("Boolean 10", "1.0")
{
    auto bool_param = BooleanParameter::create(10);

    REQUIRE(bool_param);

    bool_param->setValue(true);
    bool_param->typeDefinition().setDefaultValue(true);

    write(bool_param, false);

    write(bool_param, false);
}

TEST_CASE("String", "1.0")
{
    auto param = StringParameter::create(1);

    REQUIRE(param);

    param->setValue("true");
    param->setLabel("LA");
    param->typeDefinition().setDefaultValue("d");


    //
    write(param, false);


    param->clearLabel();
    write(param, false);


    param->setOrder(3);
    param->setEnabled(true);
    write(param, false);
}


TEST_CASE("Parameter Options reparse", "1.0")
{
    auto param = StringParameter::create(2);
    REQUIRE(param);

    param->setValue("string value");
    param->setLabel("label");
    param->setLanguageLabel("eng", "the label");
    param->setDescription("desc");
    param->setDescriptionLanguage("eng", "the description");
    param->setTags("tag1 tag2");
    param->setOrder(123);
    param->setUserdata({1, 2, 3});
    param->setUserid("userid");
    param->setReadonly(true);
    param->setEnabled(false);



    StringStreamWriter writer;
    ParameterWriter pw;

    pw.write(param, writer, false);

    auto manager = ParameterManager::create();
    auto parsed_param = ParameterParser::parse(writer.getBuffer(), manager);

    REQUIRE(parsed_param);

    auto pp = std::dynamic_pointer_cast<StringParameter>(parsed_param);

    REQUIRE(pp);
    REQUIRE(pp->getValue() == param->getValue());
    REQUIRE(pp->getLabel() == param->getLabel());
    REQUIRE(pp->getDescription() == param->getDescription());
    REQUIRE(pp->getTags() == param->getTags());
    REQUIRE(pp->getOrder() == param->getOrder());
    REQUIRE(pp->getUserdata() == param->getUserdata());
    REQUIRE(pp->getUserid() == param->getUserid());
    REQUIRE(pp->getReadonly() == param->getReadonly());
    REQUIRE(pp->getEnabled() == param->getEnabled());

    REQUIRE(pp->getLabelLanguages() == param->getLabelLanguages());
    REQUIRE(pp->getDescriptionLanguages() == param->getDescriptionLanguages());



    for (auto& l : param->getLabelLanguages())
    {
        REQUIRE(pp->getLanguageLabel(l) == param->getLanguageLabel(l));
    }

    for (auto& l : param->getDescriptionLanguages())
    {
        REQUIRE(pp->getDescriptionLanguage(l) == param->getDescriptionLanguage(l));
    }
}


TEST_CASE("Boolean type options", "1.0")
{
    auto param = BooleanParameter::create(2);
    REQUIRE(param);

    param->typeDefinition().setDefaultValue(true);

    auto pp = reparse<BooleanParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
}

TEST_CASE("Int type options", "1.0")
{
    auto param = Int32Parameter::create(2);
    REQUIRE(param);

    param->typeDefinition().setDefaultValue(77);
    param->typeDefinition().setMinimum(-100);
    param->typeDefinition().setMaximum(100);
    param->typeDefinition().setStepsize(5);
    param->typeDefinition().setUnit("unit");

    auto pp = reparse<Int32Parameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
    REQUIRE(pp->typeDefinition().getMinimum() == param->typeDefinition().getMinimum());
    REQUIRE(pp->typeDefinition().getMaximum() == param->typeDefinition().getMaximum());
    REQUIRE(pp->typeDefinition().getStepsize() == param->typeDefinition().getStepsize());
    REQUIRE(pp->typeDefinition().getUnit() == param->typeDefinition().getUnit());
}

TEST_CASE("Float type options", "1.0")
{
    auto param = Float32Parameter::create(2);
    REQUIRE(param);

    param->typeDefinition().setDefaultValue(3.1415);
    param->typeDefinition().setMinimum(-100);
    param->typeDefinition().setMaximum(100);
    param->typeDefinition().setStepsize(5);
    param->typeDefinition().setUnit("unit");

    auto pp = reparse<Float32Parameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
    REQUIRE(pp->typeDefinition().getMinimum() == param->typeDefinition().getMinimum());
    REQUIRE(pp->typeDefinition().getMaximum() == param->typeDefinition().getMaximum());
    REQUIRE(pp->typeDefinition().getStepsize() == param->typeDefinition().getStepsize());
    REQUIRE(pp->typeDefinition().getUnit() == param->typeDefinition().getUnit());
}

TEST_CASE("Double type options", "1.0")
{
    auto param = Float64Parameter::create(2);
    REQUIRE(param);

    param->typeDefinition().setDefaultValue(3.1415);
    param->typeDefinition().setMinimum(-100);
    param->typeDefinition().setMaximum(100);
    param->typeDefinition().setStepsize(5);
    param->typeDefinition().setUnit("unit");

    auto pp = reparse<Float64Parameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
    REQUIRE(pp->typeDefinition().getMinimum() == param->typeDefinition().getMinimum());
    REQUIRE(pp->typeDefinition().getMaximum() == param->typeDefinition().getMaximum());
    REQUIRE(pp->typeDefinition().getStepsize() == param->typeDefinition().getStepsize());
    REQUIRE(pp->typeDefinition().getUnit() == param->typeDefinition().getUnit());
}


TEST_CASE("Custom type options", "1.0")
{
    auto param = CustomParameter::create(2, 3);
    REQUIRE(param);

    param->typeDefinition().setTypeId(44);
    param->typeDefinition().setDefaultValue({1, 2, 3});
    param->typeDefinition().setConfig({1, 2, 3, 4, 5});

    param->setValue({5, 6, 7});

    auto pp = reparse<CustomParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->typeDefinition().getSize() == param->typeDefinition().getSize());
    REQUIRE(pp->typeDefinition().getTypeId() == param->typeDefinition().getTypeId());
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
    REQUIRE(pp->typeDefinition().getConfig() == param->typeDefinition().getConfig());

    REQUIRE(pp->getValue() == param->getValue());
}


TEST_CASE("String type options", "1.0")
{
    auto param = StringParameter::create(2);
    REQUIRE(param);

    param->typeDefinition().setDefaultValue("default value");
    param->typeDefinition().setRegex("REGEX");

    auto pp = reparse<StringParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
    REQUIRE(pp->typeDefinition().getRegex() == param->typeDefinition().getRegex());
}

TEST_CASE("IPv4", "1.0")
{
    auto param = IPv4Parameter::create(2);
    REQUIRE(param);

    param->typeDefinition().setDefaultValue(123);

    auto pp = reparse<IPv4Parameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
}


TEST_CASE("IPv6", "1.0")
{
    auto param = IPv6Parameter::create(2);
    REQUIRE(param);

    param->typeDefinition().setDefaultValue(IPv6(1, 2, 3, 4));

    auto pp = reparse<IPv6Parameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
}


TEST_CASE("Bang Parameter", "1.0")
{
    auto param = BangParameter::create(1);
    REQUIRE(param);

    param->setLabel("bang");



    bool banged = false;
    param->setBangCb([&banged]
    {
        banged = true;
    });

    // bang parameter
    param->update(BangParameter::create(1));

    REQUIRE(banged);


    auto pp = reparse<BangParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->getLabel() == param->getLabel());
}


TEST_CASE("Range", "1.0")
{
    auto param = RangeParameter<int8_t>::create(2);

    param->setValue({4, 5});
    param->typeDefinition().setDefaultValue({1, 1});
    param->typeDefinition().elementTypeDefinition().setUnit("unit");

    auto pp = reparse<RangeParameter<int8_t>>(param);

    REQUIRE(pp);
    REQUIRE(pp->getValue() == param->getValue());
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
    REQUIRE(pp->typeDefinition().elementTypeDefinition().getUnit() == param->typeDefinition().elementTypeDefinition().getUnit());
}


TEST_CASE("Color RGB", "1.0")
{
    auto param = RGBParameter::create(2);

    param->setValue(123);
    param->typeDefinition().setDefaultValue(1);

    auto pp = reparse<RGBParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->getValue() == param->getValue());
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
}


TEST_CASE("Color RGBA", "1.0")
{
    auto param = RGBAParameter::create(2);

    param->setValue(123);
    param->typeDefinition().setDefaultValue(1);

    auto pp = reparse<RGBAParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->getValue() == param->getValue());
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
}


TEST_CASE("Color float RGB", "1.0")
{
    auto param = RGBFloatParameter::create(2);

    param->setValue({0.5, 0.5, 0.5});
    param->typeDefinition().setDefaultValue({1, 1, 1});

    // write(param, false);
    // return;

    auto pp = reparse<RGBFloatParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->getValue() == param->getValue());
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
}


TEST_CASE("Color float RGBA", "1.0")
{
    auto param = RGBAFloatParameter::create(2);

    param->setValue({0.5, 0.5, 0.5, 1});
    param->typeDefinition().setDefaultValue({1, 1, 1, 1});

    auto pp = reparse<RGBAFloatParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->getValue() == param->getValue());
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
}


TEST_CASE("Uri", "1.0")
{
    auto param = URIParameter::create(2);

    param->setValue("bla");
    param->typeDefinition().setDefaultValue("url2");
    param->typeDefinition().setFilter("filter");
    param->typeDefinition().setSchemas({"http", "https", "ftp", "smb"});

    param->setValue("test");

    auto pp = reparse<URIParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->getValue() == param->getValue());
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
    REQUIRE(pp->typeDefinition().getFilter() == param->typeDefinition().getFilter());
    REQUIRE(pp->typeDefinition().getSchemas() == param->typeDefinition().getSchemas());

    REQUIRE(pp->getValue() == param->getValue());
}


}
}

