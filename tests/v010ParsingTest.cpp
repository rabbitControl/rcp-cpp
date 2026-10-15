#include <catch2/catch_test_macros.hpp>

#include <rcp/0.1.0/PacketParser.h>

#include <strstream>
#include <vector>
#include <fstream>


void _parseStreamInvalid(std::istream& stream)
{
    REQUIRE(stream.good());

    auto manager = rcp::ParameterManager::create();

    rcp::v010::PacketParser::parse(stream,
                                   manager,
                                   [](const rcp::v010::IdDataPtr& id)
    {
        REQUIRE(!id);
    },
                                   [](const rcp::v010::InfoDataPtr& info)
    {
        REQUIRE(!info);
    },
                                   [](const rcp::IParameterPtr& parameter)
    {
        REQUIRE(!parameter);
    },
                                   [](const rcp::v010::IdDataPtr& id)
    {
        REQUIRE(!id);
    },
                                   []
    {
        // v10 packet
    });
}

void _parseStream(std::istream& stream)
{
    REQUIRE(stream.good());

    auto manager = rcp::ParameterManager::create();

    rcp::v010::PacketParser::parse(stream,
                                   manager,
                                   [](const rcp::v010::IdDataPtr& id)
    {
        if (id)
        {
            std::cout << "INIT: " << std::to_string(id->getId()) << std::endl;
        }
        else
        {
            std::cout << "INIT" << std::endl;
        }
    },
                                   [](const rcp::v010::InfoDataPtr& info)
    {
        if (info)
        {
            REQUIRE(!info->getVersion().empty());
            std::cout << "VERSION: " << info->getVersion() << "::" << info->getApplicationId() << std::endl;
        }
    },
                                   [](const rcp::IParameterPtr& parameter)
    {
        REQUIRE(parameter);
        std::cout << "UPDATE PARAMETER: " << std::to_string(parameter->getId()) << std::endl;
    },
                                   [](const rcp::v010::IdDataPtr& id)
    {
        REQUIRE(id);
        REQUIRE(id->getId() != 0);
        std::cout << "REMOVE: " << std::to_string(id->getId()) << std::endl;
    },
                                   []
    {
        // v10 packet
    });
}

void parseData(std::vector<char> data)
{
    std::istrstream stream(reinterpret_cast<const char*>(data.data()), data.size());

    INFO("reading from vector with length: " << data.size());
    _parseStream(stream);
}

void parseData(const std::string& filename)
{
    std::ifstream fileis(filename);

    INFO("reading from: " << filename);
    _parseStream(fileis);
}


TEST_CASE("parsing", "0.1.0")
{
    std::vector<char> d = {0x04, 0x12, 0x00, 0x09, 0x10, 0x00, 0x21, 0x61, 0x6e, 0x79, 0x04, 0x62, 0x6f, 0x6f, 0x6c, 0x00, 0x25, 0x00, 0x07, 0x00, 0x00};
    parseData(d);
}

TEST_CASE("parsing error", "0.1.0")
{
    std::vector<char> data = {0x04, 0x12, 0x00, 0x10, 0x00, 0x21, 0x61, 0x6e, 0x79, 0x04, 0x62, 0x6f, 0x6f, 0x6c, 0x00, 0x25, 0x00, 0x07, 0x00, 0x00};

    std::istrstream stream(reinterpret_cast<const char*>(data.data()), data.size());

    INFO("reading from vector with length: " << data.size());
    _parseStreamInvalid(stream);
}


TEST_CASE("parsing rcp 0.1.0 example data", "0.1.0")
{
    const std::string path = "path/to/example_data/";

    parseData(path + "packet_info_nodata.rcp");
    parseData(path + "packet_info.rcp");

    parseData(path + "packet_initialize.rcp");
    parseData(path + "packet_initialize_id_data.rcp");

    parseData(path + "packet_bool_no_user.rcp");
    parseData(path + "packet_bool_userdata.rcp");
    parseData(path + "packet_update_bool.rcp");
    parseData(path + "packet_enum.rcp");
    parseData(path + "packet_lstr_no_user.rcp");
    parseData(path + "packet_range.rcp");
    parseData(path + "packet_s8_no_user.rcp");
    parseData(path + "packet_string_default.rcp");
    parseData(path + "packet_u32_no_user.rcp");
    parseData(path + "packet_update_u32.rcp");
    parseData(path + "packet_uri.rcp");

    // parseData(path + "packet_update_boolarray_no_user.rcp");


    parseData(path + "packet_updatevalue_s8.rcp");
    parseData(path + "packet_updatevalue_s32.rcp");
    parseData(path + "packet_updatevalue_string.rcp");
    parseData(path + "packet_update_float32.rcp");

    parseData(path + "packet_remove.rcp");
}


