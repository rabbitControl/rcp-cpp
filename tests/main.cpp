#include <cstdio>
#include <iostream>
#include <istream>
#include <strstream>
#include <fstream>


#include "src/0.1.0/ParameterDefinitions.h"
#include "src/0.1.0/GroupParameter.h"
#include "src/0.1.0/PacketParser.h"
#include "src/0.1.0/ParameterDumper.h"

#include "src/StringStreamWriter.h"

#include "src/1.0/ParameterDefinitions.h"
#include "src/1.0/ParameterWriter.h"

using namespace std;
using namespace rcp;
using namespace rcp::v010;



void _parseStream(std::istream& stream)
{
    if (!stream.good())
    {
        return;
    }

    rcp::ParameterManager manager;

    rcp::v010::PacketParser::parse(stream,
                                   &manager,
                                   [](const rcp::v010::IdDataPtr& id)
    {
        // init
        if (id)
        {
            std::cout << "INIT: " << id->getId() << std::endl;
        }
        else
        {
            std::cout << "INIT: " << std::endl;
        }
    },
                                   [](const rcp::v010::InfoDataPtr& info)
    {
        // version
        if (info)
        {
            std::cout << "VERSION v: " << info->getVersion() << " app_id:" << info->getApplicationId() << std::endl;
        }
        else
        {
            std::cout << "VERSION:" << std::endl;
        }
    },
                                   [](const rcp::IParameterPtr& param)
    {
        // update
        if (param)
        {
            std::cout << "UPDATE: " << std::to_string(param->getId()) << std::endl;
            rcp::v010::ParameterDumper::dump(param);
        }
        else
        {
            std::cout << "UPDATE" << std::endl;
        }

    },
                                   [](const rcp::v010::IdDataPtr& id)
    {
        // remove
        if (id)
        {
            std::cout << "REMOVE: " << id->getId() << std::endl;
        }
        else
        {
            std::cout << "REMOVE" << std::endl;
        }
    });

    // Option<Packet> packet_option = Packet::parse(stream);

    // if (!packet_option.hasValue()) {
    //     std::cout << "error: no packet!\n";
    //     return;
    // }

    // Packet& the_packet = packet_option.value();

    // std::cout << "packet command: " << the_packet.getCommand() << "\n";

    // if (the_packet.hasTimestamp()) {
    //     std::cout << "packet timestamp: " << the_packet.getTimestamp() << "\n";
    // }


    // switch (the_packet.getCommand()) {

    // case COMMAND_INVALID:
    // case COMMAND_MAX_:
    // case COMMAND_DISCOVER:
    // case COMMAND_INITIALIZE:
    //     std::cout << "command not handled: " << the_packet.getCommand();
    //     break;

    // case COMMAND_INFO:
    // {
    //     if (!the_packet.hasData()) {
    //         std::cout << "infopacket without data\n";
    //         return;
    //     }

    //     InfoDataPtr infodata = std::dynamic_pointer_cast<InfoData>(the_packet.getData());
    //     if (infodata) {
    //         std::cout << "version: " << infodata->getVersion() << std::endl;
    //         std::cout << "appid: " << infodata->getApplicationId() << std::endl;
    //     } else {
    //         std::cerr << "could not get data as InfoData";
    //     }
    // }
    // break;

    // case COMMAND_UPDATEVALUE:
    // case COMMAND_UPDATE:
    // {
    //     if (!the_packet.hasData()) {
    //         std::cout << "error: packet does not contain data\n";
    //         return;
    //     }

    //     // assume this is a parameter!!
    //     ParameterPtr param = std::dynamic_pointer_cast<IParameter>(the_packet.getData());
    //     if (param) {

    //         param->dump();
    //         std::cout << "---------------------\n";

    //         if (param->getDatatype() == DATATYPE_BOOLEAN) {

    //             auto p = std::dynamic_pointer_cast<BooleanParameter>(param);
    //             //                std::cout << "boolean default: " << p->getDefault() << "\n";
    //             //                std::cout << "boolean value: " << p->getValue() << "\n";

    //         } else if (param->getDatatype() == DATATYPE_INT32) {

    //             auto p = std::dynamic_pointer_cast<Int32Parameter>(param);
    //             p->getMaximum();

    //         } else if (param->getDatatype() == DATATYPE_UINT32) {

    //             auto p = std::dynamic_pointer_cast<UInt32Parameter>(param);

    //             if (p->hasValue()) {
    //                 std::cout << "value for uint32: " << p->getValue() << "\n";
    //             }
    //             if (p->getDefaultTypeDefinition().hasMaximum()) {
    //                 std::cout << "max for uint32: " << p->getMaximum() << "\n";
    //             }

    //         } else if (param->getDatatype() == DATATYPE_INT8) {

    //             auto p = std::dynamic_pointer_cast<Int8Parameter>(param);
    //             p->getMultipleof();

    //         } else if (param->getDatatype() == DATATYPE_ENUM) {

    //             auto p = std::dynamic_pointer_cast<EnumParameter>(param);
    //             std::cout << "selected option: " << p->getDefaultTypeDefinition().getOption(p->getValue()) << "\n";

    //         } else if (param->getDatatype() == DATATYPE_STRING) {

    //             auto p = std::dynamic_pointer_cast<StringParameter>(param);
    //             p->getDefaultTypeDefinition().getRegex();

    //         } else if (param->getDatatype() == DATATYPE_RANGE) {

    //             auto p = std::dynamic_pointer_cast<IElementParameter>(param);
    //             std::cout << "element type: " << p->getElementType() << "\n";

    //             // would need to switch on element_type to get real RageParameter<T>
    //         }
    //     }
    // }
    // break;

    // case COMMAND_REMOVE:
    // {
    //     if (!the_packet.hasData()) {
    //         std::cout << "remove packet does not contain data\n";
    //         return;
    //     }

    //     // assume this is a id-data
    //     IdDataPtr data = std::dynamic_pointer_cast<IdData>(the_packet.getData());
    //     if (data) {
    //         std::cout << "id: " << data->getId();
    //     }

    //     break;
    // }
    // }
}

void parseData(std::vector<char> data)
{
    std::istrstream stream(reinterpret_cast<const char*>(data.data()), data.size());
    _parseStream(stream);
}

void parseData(const std::string& filename)
{
    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    std::cout << "----------------------\n";
    std::cout << "reading from: " << filename << "\n";
    std::flush(std::cout);


    std::ifstream fileis(filename);
    if (fileis.good())
    {
        _parseStream(fileis);
    }
    else
    {
        std::cerr << "error: file does not exist: " << filename << "\n";
    }

    std::cout << "\n\n";

    std::flush(std::cout);
    std::flush(std::cerr);
}


int main(int argc, char**argv)
{
    auto bool_param_0 = v010::BooleanParameter::create(10);
    bool_param_0->setValue(true);
    bool_param_0->typeDefinition().setDefaultValue(true);

    return 0;

    // 1.0
    auto bang_10 = rcp::v10::BangParameter::create(1);
    auto group_10 = rcp::v10::GroupParameter::create(1);
    auto bool_10 = rcp::v10::BooleanParameter::create(1);


    StringStreamWriter writer_10;
    rcp::v10::ParameterWriter::write(bool_10, writer_10, true);

    writer_10.dump();

    return 0;



    std::vector<char> d = {0x04, 0x12, 0x00, 0x09, 0x10, 0x00, 0x21, 0x61, 0x6e, 0x79, 0x04, 0x62, 0x6f, 0x6f, 0x6c, 0x00, 0x25, 0x00, 0x07, 0x00, 0x00};
    parseData(d);

    parseData("../../../../../RCP/example_data/packet_update_float32.rcp");
    parseData("../../../../../RCP/example_data/packet_updatevalue_s8.rcp");
    parseData("../../../../../RCP/example_data/packet_updatevalue_s32.rcp");
    parseData("../../../../../RCP/example_data/packet_updatevalue_string.rcp");
    parseData("../../../../../RCP/example_data/packet_remove.rcp");
    parseData("../../../../../RCP/example_data/packet_bool_no_user.rcp");

    parseData("../../../../../RCP/example_data/packet_info_nodata.rcp");
    parseData("../../../../../RCP/example_data/packet_info.rcp");

    parseData("../../../../../RCP/example_data/packet_bool_no_user.rcp");
    parseData("../../../../../RCP/example_data/packet_bool_userdata.rcp");
    parseData("../../../../../RCP/example_data/packet_update_bool.rcp");
    // parseData("../../../../../RCP/example_data/packet_update_boolarray_no_user.rcp");
    parseData("../../../../../RCP/example_data/packet_enum.rcp");
    parseData("../../../../../RCP/example_data/packet_initialize.rcp");
    parseData("../../../../../RCP/example_data/packet_initialize_id_data.rcp");

    parseData("../../../../../RCP/example_data/packet_lstr_no_user.rcp");
    parseData("../../../../../RCP/example_data/packet_range.rcp");
    parseData("../../../../../RCP/example_data/packet_s8_no_user.rcp");
    parseData("../../../../../RCP/example_data/packet_string_default.rcp");
    parseData("../../../../../RCP/example_data/packet_u32_no_user.rcp");
    parseData("../../../../../RCP/example_data/packet_update_u32.rcp");
    parseData("../../../../../RCP/example_data/packet_uri.rcp");


    return 0;

    auto param = v010::StringParameter::create(10);
    param->setLabel("string label");
    param->setValue("Test value");    
    param->typeDefinition().setRegex("test");
    param->typeDefinition().setDefaultValue("default TEST");


    param->addValueUpdatedCb([](const std::string& value)
    {
        std::cout << "string value changed:" << value << std::endl;
    });


    auto param_2 = v010::StringParameter::create(10);
    param_2->setLabel("string label 2");
    param_2->setValue("Test value 1");
    param_2->typeDefinition().setRegex("test 1");
    param_2->typeDefinition().setDefaultValue("default TEST 1");


    param->update(param_2);



    vector<uint8_t> data = {1, 2, 3};

    // need some way to create istream from data
    std::istrstream stream(reinterpret_cast<const char*>(data.data()), data.size());

    param->parseTypeOptions(stream);



    printf("label: %s\n", param->getLabel().c_str());
    printf("value: %s\n", param->getValue().c_str());
    printf("default: %s\n", param->typeDefinition().getDefaultValue().c_str());

    if (param->typeDefinition().hasRegex())
    {
        printf("regex: %s\n", param->typeDefinition().getRegex().c_str());
    }


    printf("\n");


    auto bang = v010::BangParameter::create(2);
    bang->setBangCb([]{
        std::cout << "banf\n";
    });


    auto bool_param = v010::BooleanParameter::create(10);
    bool_param->setValue(true);
    bool_param->typeDefinition().setDefaultValue(true);



    auto group = v010::GroupParameter::create(1);

    group->addChild(param);
    group->addChild(bang);



    auto str = group->childAs<v010::StringParameter>(10);
    if (str)
    {
        std::cout << "str:" << str->getValue() << std::endl;
    }
    else
    {
        std::cout << "no str\n";
    }


    group->removeChild(bang);
    group->removeChild(param);



    param->update(bool_param);




    // numbers
    auto int_param = v010::Int32Parameter::create(1);
    int_param->typeDefinition().setMinimum(0);
    int_param->typeDefinition().setMaximum(100);
    int_param->setLabel("AAA");
    int_param->setOrder(2);
    int_param->setValue(4);




    auto range = v010::RangeParameter<int32_t>::create(1);
    range->typeDefinition().setDefaultValue(Range<int32_t>(1, 1));
    range->typeDefinition().elementTypeDefinition().setMinimum(1);
    range->typeDefinition().elementTypeDefinition().setMaximum(10);
    range->setLabel("Range");
    range->setValue(Range<int32_t>(3, 3));


    auto ipv4 = v010::IPv4Parameter::create(1);
    ipv4->setValue(123);
    ipv4->typeDefinition().setDefaultValue(0);



    auto custom = v010::CustomParameter::create(1, 3);
    // custom->typeDefinition().setDefaultValue({1, 2, 3});
    custom->setValue({4, 5, 6});



    StringStreamWriter writer;
    v010::ParameterWriter::write(custom, writer, true);

    writer.dump();


    return 0;
}
