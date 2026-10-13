#include <catch2/catch_test_macros.hpp>

#include "src/1.0/InfoData.h"
#include "src/1.0/RcpVersion.h"
#include "src/StringStreamWriter.h"
#include "src/1.0/RcpInt.h"
#include "src/1.0/RcpString.h"
#include "src/1.0/RcpLanguageString.h"

namespace rcp {
namespace v10 {

static inline void testRcpInt(int32_t value)
{
    StringStreamWriter writer;

    RcpInt(value).write(writer, true);

    REQUIRE(RcpInt::parse(writer.getBuffer()).value() == value);
}

TEST_CASE("RcpInt")
{
    testRcpInt(0);
    testRcpInt(1);
    testRcpInt(123);
    testRcpInt(0x2821);
    testRcpInt(123456789);
    testRcpInt(pow(2, 28) -1);
}


TEST_CASE("RcpInt invalid", "1.0")
{
    int32_t value = pow(2, 28);

    StringStreamWriter writer;

    try
    {
        // expect this to throw
        RcpInt(value).write(writer, true);

        // should not come here
        REQUIRE(false);
    }
    catch(std::runtime_error& ex)
    {
        REQUIRE(true);
    }
}


TEST_CASE("RcpInt buffer underrun", "1.0")
{
    StringStreamWriter writer;

    writer.write(char(1));
    writer.write(char(2));
    writer.write(char(3));

    try
    {
        // this should fail
        RcpInt::parse(writer.getBuffer()).value();

        REQUIRE(false);
    }
    catch(std::runtime_error& ex)
    {
        REQUIRE(true);
    }
}


TEST_CASE("RcpInt unterminated", "1.0")
{
    StringStreamWriter writer;

    writer.write(char(1));
    writer.write(char(2));
    writer.write(char(3));
    writer.write(char(4)); // untermintated

    try
    {
        // this should fail
        RcpInt::parse(writer.getBuffer()).value();

        REQUIRE(false);
    }
    catch(std::runtime_error& ex)
    {
        REQUIRE(true);
    }
}

TEST_CASE("RcpVersion", "1.0")
{
    RcpVersion version(1, 2);

    StringStreamWriter writer;
    version.write(writer, true);

    writer.dump();

    auto parsed_version = RcpVersion::parse(writer.getBuffer());
    REQUIRE(parsed_version == version);

    RcpVersion version1(1, 3);
    RcpVersion version2(2, 3);

    REQUIRE(version < version1);
    REQUIRE(version1 > version);
    REQUIRE(version1 >= version);
    REQUIRE(version2 > version);
    REQUIRE(version2 > version1);
    REQUIRE(version1 < version2);
    REQUIRE(version1 <= version2);

    RcpVersion version3(1, 3);

    REQUIRE(version1 <= version3);
    REQUIRE(version3 >= version1);
}

TEST_CASE("InfoData", "1.0")
{
    // send info to client
    InfoData info(RcpVersion{1, 2}, RcpVersion{1, 2});
    info.setApplicationId("c++");
    info.setApplicationVersion("v1.0");

    StringStreamWriter writer;
    info.write(writer, true);

    writer.dump();


    auto parsed_info = InfoData::parse(writer.getBuffer());

    REQUIRE(parsed_info.protocolVersion() == info.protocolVersion());
    REQUIRE(parsed_info.handshakeVersion() == info.handshakeVersion());
    REQUIRE(parsed_info.applicationId() == info.applicationId());
    REQUIRE(parsed_info.applicationVersion() == info.applicationVersion());
}


TEST_CASE("RcpString", "1.0")
{
    RcpString value("value");

    StringStreamWriter writer;
    value.write(writer, true);

    REQUIRE(RcpString::parse(writer.getBuffer()) == value);
}



TEST_CASE("Language String", "1.0")
{
    StringStreamWriter writer;
    RcpLanguageString l("any", {{"deu", "eines"}, {"eng", "some"}});

    l.write(writer, true);

    auto lng = RcpLanguageString::parse(writer.getBuffer());

    REQUIRE(lng.anyString() == l.anyString());
    REQUIRE(lng.languageMap().at("deu") == l.languageMap().at("deu"));
    REQUIRE(lng.languageMap().at("eng") == l.languageMap().at("eng"));
}


TEST_CASE("STR", "1.0")
{
    std::vector<std::string> m_schemas;

    std::stringstream strstr("uno dos tres");
    std::istream_iterator<std::string> begin(strstr);
    std::istream_iterator<std::string> end;

    m_schemas.assign(begin, end);

    REQUIRE(m_schemas.size() == 3);
    REQUIRE(m_schemas[0] == "uno");
    REQUIRE(m_schemas[1] == "dos");
    REQUIRE(m_schemas[2] == "tres");
}


}
}
