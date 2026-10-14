#include <catch2/catch_test_macros.hpp>

#include <rcp/1.0/ParameterWriter.h>
#include <rcp/1.0/ParameterDefinitions.h>

#include "1.0/Helper.h"

namespace rcp {
namespace v10 {


TEST_CASE("Enum type options", "1.0")
{
    auto param = EnumParameter::create(2);
    REQUIRE(param);

    param->typeDefinition().addOption({"option 1", {{"eng", "first choice"}, {"deu", "Die erste Wahl"}}});
    param->typeDefinition().addOption({"option 2", {{"eng", "second choice"}, {"deu", "Die zweite Wahl"}}});
    param->typeDefinition().setMinimumSelectionCount(1);
    param->typeDefinition().setMaximumSelectionCount(10);
    param->typeDefinition().setDefaultValue({1, 2, 3});

    param->setValue({1, 2, 3});


    auto pp = reparse<EnumParameter>(param);

    REQUIRE(pp);
    REQUIRE(pp->typeDefinition().getDefaultValue() == param->typeDefinition().getDefaultValue());
    REQUIRE(pp->typeDefinition().getMinimumSelectionCount() == param->typeDefinition().getMinimumSelectionCount());
    REQUIRE(pp->typeDefinition().getMaximumSelectionCount() == param->typeDefinition().getMaximumSelectionCount());
    REQUIRE(pp->typeDefinition().getAnyOptions() == param->typeDefinition().getAnyOptions());
    REQUIRE(pp->typeDefinition().getLanguageOptions("eng") == param->typeDefinition().getLanguageOptions("eng"));
    REQUIRE(pp->typeDefinition().getLanguageOptions("deu") == param->typeDefinition().getLanguageOptions("deu"));

    REQUIRE(pp->getValue() == param->getValue());


    // for (auto& entry : pp->typeDefinition().getAnyOptions())
    // {
    //     std::cout << "any: " << entry << std::endl;
    // }

    // for (auto& entry : pp->typeDefinition().getLanguageOptions("eng"))
    // {
    //     std::cout << "english: " << entry << std::endl;
    // }

    // for (auto& entry : pp->typeDefinition().getLanguageOptions("deu"))
    // {
    //     std::cout << "german: " << entry << std::endl;
    // }
}



}
}

