/*
********************************************************************
* rabbitcontrol cpp
*
* written by: Ingo Randolf - 2018
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************
*/

#include <string>
#include <fstream>

#include "src/rcp.h"

#include "dummyserver.h"
#include "dummyclient.h"
#include "src/infodata.h"


using namespace rcp;

void testHierarchy() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    ParameterServer server;

    std::cout << "----------- creating parameter\n";
    rcp::StringParameter& sp = server.createStringParameter("string uno");
    rcp::BooleanParameter& bool1 = server.createBooleanParameter("bool 1");

    std::cout << "----------- adding parameter\n";
    server.addParameter(sp);

    std::cout << "----------- creating group1\n";
    GroupParameterPtr group = server.createGroupParameter("group1");
    GroupParameterPtr group2 = server.createGroupParameter("group2");

    std::cout << "----------- add parameter to group1\n";
    server.addParameter(sp, group);

    std::cout << "----------- creating boolean\n";
    auto boolParam = server.createBooleanParameter("boolean", group);
    auto boolParam2 = server.createBooleanParameter("boolean 2", group2);
    auto intParam = server.createInt32Parameter("the integer", group2);

    auto intParam2 = server.createInt32Parameter("another integer");

    std::cout << "----------- hierarchy\n";
    server.dumpHierarchy();

    std::cout << "----------- \n";
    std::weak_ptr<GroupParameter>& gg = server.getRoot()->getParent();
//    std::cout << gg.getId();



    // e.g. store parameters in a list
    std::vector<std::reference_wrapper<IParameter> > params;
    params.push_back(boolParam);
    params.push_back(intParam);

    // iterate list and change labels
    int i=0;
    for (const auto& p : params) {
        std::cout << "parameter: " << p.get().getId() << ": " << p.get().getLabel() << "\n";
        p.get().setLabel("bla" + std::to_string(i));
        i++;
    }

    // check changed labels
    std::cout << "int: " << intParam.getLabel() << "\n";
    std::cout << "bool: " << boolParam.getLabel() << "\n";

    std::cout << "\n\n";
}


void testUpdatedCb() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    // create parameter to be updated
    Int32Parameter param(1);
    param.setLabel("original label");

    // create parameter to update with (use same id as param)
    Int32Parameter param2(1);
    param2.setLabel("new label from param2");

    // set callbacks on parameter to be updated
    auto& cb1 = param.addUpdatedCb([&]() {
        std::cout << "callback 1 - parameter id: " << param.getId() << "\n";
        std::cout << "\tlabel: " << param.getLabel() << "\n";
        std::cout << "\tdescription: " << param.getDescription() << "\n";
    });
    auto& cb2 = param.addUpdatedCb([&]() {
        std::cout << "callback 2 - parameter id: " << param.getId() << "\n";
        std::cout << "\tlabel: " << param.getLabel() << "\n";
        std::cout << "\tdescription: " << param.getDescription() << "\n";
    });


    // update param with param2
    std::cout << "-------\n";
    param.update(param2.newReference());

    // adjust param2 and update
    std::cout << "-------\n";
    param2.setDescription("parameter description");
    param.update(param2.newReference());

    // remove callback 1 and update again
    std::cout << "-------\n";
    std::cout << "remove cb1\n";
    param.removeUpdatedCb(cb1);
    param.update(param2.newReference());

    std::cout << "\n\n";
}


void testValueUpdateCb1() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    BooleanParameter param(1);
    BooleanParameter param2(1);

    std::function< void(bool&) > func = [](bool& v) {
        std::cout << "param value updated (lvalue): " << (v ? "true" : "false") << "\n";
    };

    param.addValueUpdatedCb([](bool& v) {
        std::cout << "param value updated: " << (v ? "true" : "false") << "\n";
    });

    auto& cbl = param.addValueUpdatedCb(func);

    param2.setValue(true);
    param.update(param2.newReference());

    param.removeValueUpdatedCb(cbl);

    param2.setValue(false);
    param.update(param2.newReference());

    std::cout << "\n\n";
}


void testValueUpdateCb2() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    BooleanParameter param(1);
    BooleanParameter param2(1);

    auto& cb1 = param.addValueUpdatedCb([](bool& v) {
        std::cout << "param value updated 1: " << (v ? "true" : "false") << "\n";
    });   

    param.addValueUpdatedCb([](bool& v) {
        std::cout << "param value updated 2: " << (v ? "true" : "false") << "\n";
    });

    param2.setValue(true);
    param.update(param2.newReference());

    //
    std::cout << "---------\n";
    param.removeValueUpdatedCb(cb1);
    param2.setValue(false);
    param.update(param2.newReference());

    std::cout << "\n\n";
}


void testTransporter() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    ParameterServer server;
    DummyServerTransporter serverTransporter;
    serverTransporter.addConnection();
    serverTransporter.addConnection();

    ServerUser user1(serverTransporter, "user1");
    ServerUser user2(serverTransporter, "user2");
    serverTransporter.testcallall("test1");
    user2.removeCb();
    serverTransporter.testcallall("test2");


//    DummyClientTransporter clientTransporter;
//    ClientTest client1(clientTransporter, "client1");
//    ClientTest client2(clientTransporter, "client2");
//    ClientTest client3(clientTransporter, "client3");
//    clientTransporter.testcallall("blubb");
//    client1.removeCb();
//    clientTransporter.testcallall("blaa");



    server.addTransporter(serverTransporter);
    server.addTransporter(serverTransporter);


    std::cout << "connections: " << server.getConnectionCount() << "\n";

    server.removeTransporter(serverTransporter);
    std::cout << "connections: " << server.getConnectionCount() << "\n";

    std::cout << "\n\n";
}

void testUpdate() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    ParameterServer server;
    DummyServerTransporter serverTransporter;

    server.addTransporter(serverTransporter);

    ServerUser user2(serverTransporter, "user2");

    auto group = server.createGroupParameter("g");

    auto& sp = server.createInt8Parameter("i8", group);
    sp.setValue(10);

    auto& bang = server.createBangParameter("bang", group);
    bang.setDescription("desc");


    server.update();

    serverTransporter.testcallall("blubb");

    std::cout << "\n\n";
}


void testInt8Parameter() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    Int8Parameter param(1, 32);
    param.setLabel("param label");
    param.setDescription("param description");
    param.setMinimum(-1);
    param.setMaximum(111);
    param.setMultipleof(2);
    param.setScale(NUMBER_SCALE_LOGARITHMIC);
    param.setUnit("CM");

    StringStreamWriter writer;
    param.write(writer, true);

    std::cout << "Int8Parameter:\n";
    writer.dump();

    std::cout << "\n\n";
}

void testEnumParam() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    EnumParameter param(1);
    param.getDefaultTypeDefinition().addOption("test1");
    param.getDefaultTypeDefinition().addOption("test2");
    param.getDefaultTypeDefinition().addOption("test3");
    param.setValue("test2");

    StringStreamWriter writer;
    param.write(writer, true);

    std::cout << "EnumParameter:\n";
    writer.dump();

    std::cout << "\n\n";
}

void testRangeParameter() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    RangeParameter<int> param(1);
    param.setValue(rcp::Range<int>(2, 3));
    param.setDefault(rcp::Range<int>(1, 2));
    param.setElementDefault(4);
    param.setMinimum(1);
    param.setMaximum(5);

    StringStreamWriter writer;
    param.write(writer, true);

    std::cout << "RangeParameter:\n";
    writer.dump();

    std::cout << "\n\n";
}

void testStringParameter() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    StringParameter param(1);
    param.getDefaultTypeDefinition().setDefault("aa");
    param.getDefaultTypeDefinition().setRegex("**");

    StringStreamWriter writer;
    param.write(writer, true);

    std::cout << "StringParameter:\n";
    writer.dump();

    std::cout << "\n\n";
}


void testCustomParameter() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    char uuid[16];
    for (char i=0; i<16; i++) {
        uuid[i] = i+60;
    }

    std::vector<int8_t> config;
    config.push_back(5);
    config.push_back(6);
    config.push_back(7);

    CustomParameter<int> param(1);
    param.setUuid(uuid, 16);
    param.setConfig(config);

    StringStreamWriter writer;
    writer.clear();
    param.write(writer, true);

    std::cout << "CustomParameter:\n";
    writer.dump();

    std::cout << "\n\n";
}


void testUriParameter() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    auto e = URIParameter::create(1);
    e->getDefaultTypeDefinition().addSchema("file");
    e->getDefaultTypeDefinition().addSchema("http");
    e->getDefaultTypeDefinition().addSchema("ftp");
    e->getDefaultTypeDefinition().setFilter("Text files (*.txt)|*.txt|All files (*.*)|*.*");
    e->setDefault("default-string");
    e->setLabel("uri label");
    e->setDescription("uri description");
    e->setTags("tag1 tag2");

    StringStreamWriter writer;
    e->write(writer, true);

    std::cout << "UriParameter:\n";
    writer.dump();

    std::cout << "\n\n";
}


void _parseData(const std::string& filename) {

    std::cout << "reading from: " << filename << "\n";

    std::ifstream fileis(filename);

    if (!fileis.good()) {
        std::cout << "error: file does not exist: " << filename << "\n";
        return;
    }


    Option<Packet> packet_option = Packet::parse(fileis);

    if (!packet_option.hasValue()) {
        std::cout << "error: no packet!\n";
        return;
    }

    Packet& the_packet = packet_option.getValue();

    std::cout << "packet command: " << the_packet.getCommand() << "\n";

    if (the_packet.hasTimestamp()) {
        std::cout << "packet timestamp: " << the_packet.getTimestamp() << "\n";
    }


    switch (the_packet.getCommand()) {

    case COMMAND_INVALID:
    case COMMAND_MAX_:
    case COMMAND_DISCOVER:
    case COMMAND_INITIALIZE:
    case COMMAND_UPDATEVALUE:
        std::cout << "command not handled: " << the_packet.getCommand();
        break;

    case COMMAND_INFO:
    {
        if (!the_packet.hasData()) {
            std::cout << "infopacket without data\n";
            return;
        }

        InfoDataPtr infodata = std::dynamic_pointer_cast<InfoData>(the_packet.getData());
        if (infodata) {
            std::cout << "version: " << infodata->getVersion() << std::endl;
            std::cout << "appid: " << infodata->getApplicationId() << std::endl;
        } else {
            std::cerr << "could not get data as InfoData";
        }
    }
        break;

    case COMMAND_UPDATE:
    case COMMAND_REMOVE:
    {
        if (!the_packet.hasData()) {
            std::cout << "error: packet does not contain data\n";
            return;
        }

        // assume this is a parameter!!
        ParameterPtr param = std::dynamic_pointer_cast<IParameter>(the_packet.getData());
        if (param) {

            param->dump();
            std::cout << "---------------------\n";

            if (param->getDatatype() == DATATYPE_BOOLEAN) {

                auto p = std::dynamic_pointer_cast<BooleanParameter>(param);
                p->getDefault();

            } else if (param->getDatatype() == DATATYPE_INT32) {

                auto p = std::dynamic_pointer_cast<Int32Parameter>(param);
                p->getMaximum();

            } else if (param->getDatatype() == DATATYPE_UINT32) {

                auto p = std::dynamic_pointer_cast<UInt32Parameter>(param);

                if (p->hasValue()) {
                    std::cout << "value for uint32: " << p->getValue() << "\n";
                }
                if (p->getDefaultTypeDefinition().hasMaximum()) {
                    std::cout << "max for uint32: " << p->getMaximum() << "\n";
                }

            } else if (param->getDatatype() == DATATYPE_INT8) {

                auto p = std::dynamic_pointer_cast<Int8Parameter>(param);
                p->getMultipleof();

            } else if (param->getDatatype() == DATATYPE_ENUM) {

                auto p = std::dynamic_pointer_cast<EnumParameter>(param);
                std::cout << "selected option: " << p->getDefaultTypeDefinition().getOption(p->getValue()) << "\n";

            } else if (param->getDatatype() == DATATYPE_STRING) {

                auto p = std::dynamic_pointer_cast<StringParameter>(param);
                p->getDefaultTypeDefinition().getRegex();

            } else if (param->getDatatype() == DATATYPE_RANGE) {

                auto p = std::dynamic_pointer_cast<IElementParameter>(param);
                std::cout << "element type: " << p->getElementType() << "\n";

                // would need to switch on element_type to get real RageParameter<T>
            }
        }
    }
        break;
    }

}

void parseData(const std::string& filename) {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    std::cout << "----------------------\n";
    _parseData(filename);
    std::flush(std::cout);
    std::flush(std::cerr);

    std::cout << "\n\n";
}


void testInfoData() {

    // serialize
    Packet packet(COMMAND_INFO);
    InfoDataPtr info_data = std::make_shared<InfoData>("0.0.0", "test");
    packet.setData(info_data);

    StringStreamWriter writer;
    packet.write(writer, true);

    std::cout << "infodata packet:\n";
    writer.dump();
}


//-------------------------------
//-------------------------------
int main(int argc, char const *argv[]) {

    testHierarchy();
    testTransporter();

    testUpdatedCb();
    testValueUpdateCb1();
    testValueUpdateCb2();
    testUpdate();

    testInt8Parameter();
    testEnumParam();
    testRangeParameter();
    testStringParameter();
    testCustomParameter();
    testUriParameter();

    testInfoData();
    parseData("../RCP/example_data/packet_info_nodata.rcp");
    parseData("../RCP/example_data/packet_info.rcp");

    parseData("../RCP/example_data/packet_bool_no_user.rcp");
    parseData("../RCP/example_data/packet_bool_remove.rcp");
    parseData("../RCP/example_data/packet_bool_update.rcp");
    parseData("../RCP/example_data/packet_boolarray_no_user_update.rcp");
    parseData("../RCP/example_data/packet_enum.rcp");
    parseData("../RCP/example_data/packet_initialize.rcp");
    parseData("../RCP/example_data/packet_lstr_no_user.rcp");
    parseData("../RCP/example_data/packet_range.rcp");
    parseData("../RCP/example_data/packet_s8_no_user.rcp");
    parseData("../RCP/example_data/packet_string_default.rcp");
    parseData("../RCP/example_data/packet_u32_no_user.rcp");
    parseData("../RCP/example_data/packet_u32_update.rcp");
    parseData("../RCP/example_data/packet_uri.rcp");


    return 0;
}

