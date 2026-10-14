#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <filesystem>
#include <string>

#include <iostream>
#include <nlohmann/json.hpp>
#include <strstream>

#include <rcp/1.0/IParameter.h>
#include <rcp/1.0/PacketParser.h>
#include <rcp/1.0/ParameterDefinitions.h>

using namespace nlohmann;


void test_file(const std::filesystem::path& path)
{
    std::cout << path << std::endl;

    std::ifstream ifs(path);
    json jf = json::parse(ifs);

    bool is_valid = jf["valid"];
    std::vector<char> data = jf["data"];

    if (is_valid &&
        !data.empty())
    {
        // std::cout << "data size: " << data.size() << std::endl;

        auto param_j = jf["parameter"];
        if (param_j.is_object())
        {
            // for (auto& el : param_j.items())
            // {
            //     std::cout << el.key() << '\n';
            // }
        }


        std::istrstream stream(reinterpret_cast<const char*>(data.data()), data.size());


        auto manager = rcp::ParameterManager::create();

        rcp::v10::PacketParser::parse(stream,
                                      manager,
                                      [](const std::vector<int32_t>& ids)
        {
            // init
        },
                                      [](const rcp::v10::InfoData& info)
        {
            // version
            std::cout << "VERSION: " << info << std::endl;
        },
                                      [param_j, manager](const std::vector<rcp::v10::IParameterPtr>& params, bool updateValue)
        {
            REQUIRE(params.size() == 1);

            auto param = *params.begin();

            REQUIRE(param->getId() == param_j["id"]);
            REQUIRE(param->getDatatype() == param_j["datatype"]);


            if (param_j.contains("type_options"))
            {
                // TODO
                // check type options
            }

            if (param_j.contains("value"))
            {
                // TODO
            }
            if (param_j.contains("label"))
            {
                REQUIRE(param_j["label"] == param->getLabel());
            }
            if (param_j.contains("description"))
            {
                REQUIRE(param_j["description"] == param->getDescription());
            }
            if (param_j.contains("tags"))
            {
                REQUIRE(param_j["tags"] == param->getTags());
            }
            if (param_j.contains("order"))
            {
                REQUIRE(param_j["order"] == param->getOrder());
            }
            if (param_j.contains("parentid"))
            {
                REQUIRE(manager->missingParents.size() == 1);
                REQUIRE(param_j["parentid"] == manager->missingParents.begin()->first);
            }
            if (param_j.contains("userdata"))
            {
                REQUIRE(param_j["userdata"] == param->getUserdata());
            }
            if (param_j.contains("userid"))
            {
                REQUIRE(param_j["userid"] == param->getUserid());
            }
            if (param_j.contains("readonly"))
            {
                REQUIRE(param_j["readonly"] == param->getReadonly());
            }
            if (param_j.contains("enabled"))
            {
                REQUIRE(param_j["enabled"] == param->getEnabled());
            }
        },
                                      [](const std::vector<int32_t>& ids)
        {
            // remove
            for(auto i : ids)
            {
                std::cout << "REMOVE: " << std::to_string(i) << std::endl;
            }
        },
                                      []
        {
            // version v010
            std::cout << "version v010: " << std::endl;
        });
    }
}


TEST_CASE("json parsing", "1.0")
{
    std::string path = "path/to/testdata";

    REQUIRE(std::filesystem::is_directory(path));

    for (const auto& dirEntry : std::filesystem::directory_iterator(path))
    {
        test_file(dirEntry.path());
    }
}


// TEST_CASE("json parsing - one file", "1.0")
// {
//     std::string fp = "pathto/testfile.json";

//     std::filesystem::path path(fp);

//     REQUIRE(std::filesystem::exists(path));

//     test_file(path);
// }
