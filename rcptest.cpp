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
#include <istream>
#include <strstream>
#include <cassert>
#include <thread>

#include "src/parameterclient.h"

#include "src/rcp.h"

#include "dummyserver.h"
#include "dummyclient.h"
#include "src/infodata.h"


using namespace rcp;

void testHierarchy() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    ParameterServer server;

    std::cout << "----------- creating parameter\n";
    rcp::StringParameterPtr sp = server.createStringParameter("string uno");
    rcp::BooleanParameterPtr bool1 = server.createBooleanParameter("bool 1");

    std::cout << "----------- creating group1\n";
    GroupParameterPtr group = server.createGroupParameter("group1");
    GroupParameterPtr group2 = server.createGroupParameter("group2");

    std::cout << "----------- creating boolean\n";
    auto boolParam = server.createBooleanParameter("boolean", group);
    auto boolParam2 = server.createBooleanParameter("boolean 2", group2);
    auto intParam = server.createInt32Parameter("the integer", group2);

    auto intParam2 = server.createInt32Parameter("another integer");

    std::cout << "----------- hierarchy\n";
    server.dumpHierarchy();

    std::cout << "----------- \n";


    // e.g. store parameters in a list
    std::vector<ParameterPtr> params;
    params.push_back(boolParam);
    params.push_back(intParam);

    // iterate list and change labels
    int i=0;
    for (const auto& p : params) {
        std::cout << "parameter: " << p.get()->getId() << ": " << p.get()->getLabel() << "\n";
        p.get()->setLabel("bla" + std::to_string(i));
        i++;
    }

    // check changed labels
    std::cout << "int: " << intParam->getLabel() << "\n";
    std::cout << "bool: " << boolParam->getLabel() << "\n";

    std::cout << "\n\n";
}


class ParameterUpdateListener : public rcp::ParameterListener
{
public:
    // rcp::ParameterListener
    void onParentChanged(IParameter* parent, const int16_t oldParentId) override
    {
        if (parent)
        {
            std::cout << "---- parent changed: " << oldParentId << " -> " << parent->getId() << "\n";
        }
    }
};


class ClientListener : public rcp::ParameterClientListener
{
public:
    void serverInfoReceived(const std::string& applicationId, const std::string& version) override
    {
        std::cout << "server: " << applicationId << " - version: " << version << "\n";
    }

    void parameterAdded(ParameterPtr parameter) override
    {
        std::cout << "parameter added: " << parameter->getLabel() << " : isgroup: " << (parameter->getDatatype() == DATATYPE_GROUP) << "\n";

        switch (parameter->getDatatype())
        {
        case DATATYPE_BOOLEAN:
        {
            parameter->addUpdateListener(&listener);
            m_bool = std::dynamic_pointer_cast<BooleanParameter>(parameter);

//            auto param = static_cast<rcp::BooleanParameter*>(parameter.get());
            //                std::cout << "flip bool value: " << param->getValue() << "\n";
            //                param->setValue(!param->getValue());

            if (m_group)
            {
                m_group->addChild(parameter);
            }

            break;
        }
        case DATATYPE_GROUP:
            if (parameter->getId() == 0x03)
            {
                m_group = std::dynamic_pointer_cast<GroupParameter>(parameter);
            }

            break;

        default:
            std::cout << "not handled datatype: " << parameter->getDatatype() << "\n";
            break;
        }
    }

    void parameterRemoved(ParameterPtr parameter) override
    {
        std::cout << "parameter removed: " << parameter->getId() << "\n";
    }

    void parsingError() override
    {
        std::cout << "parsing error\n";
    }

    BooleanParameterPtr m_bool;
    GroupParameterPtr m_group;

    ParameterUpdateListener listener;
};


void testClientParents()
{
    DummyClientTransporter transporter;
    ParameterClient client(transporter);

    ClientListener listener;
    client.addListener(&listener);


    GroupParameterPtr group = GroupParameter::create(0x07);
    group->setLabel("test");

    GroupParameterPtr group1 = GroupParameter::create(0x03);
    group1->setLabel("group1");

    StringStreamWriter writer;

    Packet packet(COMMAND_UPDATE);

    // add a group
    packet.setData(group);
    packet.write(writer, true);
    transporter.receive(writer.getBuffer());

    // add another group
    packet.setData(group1);
    packet.write(writer, true);
    transporter.receive(writer.getBuffer());

    // add a child
    std::cout << "\nadd client\n";
    std::cout << "----------------------------\n";
    std::vector<char> data = {0x04, 0x12, 0x00, 0x09, 0x10, 0x00, 0x21, 0x61, 0x6e, 0x79, 0x04, 0x62, 0x6f, 0x6f, 0x6c, 0x00, 0x25, 0x00, 0x07, 0x00, 0x00};
    std::istrstream stream(reinterpret_cast<const char*>(data.data()), data.size());
    transporter.receive(stream);
    client.dump();


    std::cout << "\nupdate client\n";
    std::cout << "----------------------------\n";
    client.update();
    client.dump();

//    if (listener.m_group && listener.m_bool)
//    {
//        listener.m_group->addChild(listener.m_bool);
//    }

//    std::cout << "----------------------------\n";
//    client.dump();

    // update bool value
//    data = {0x04, 0x12, 0x00, 0x09, 0x10, 0x00, 0x20, 0x01, 0x00, 0x00};
//    stream = std::istrstream(reinterpret_cast<const char*>(data.data()), data.size());
//    transporter.receive(stream);

//    std::cout << "----------------------------\n";
//    client.dump();


    // move bool to root group
//    data = {0x04, 0x12, 0x00, 0x09, 0x10, 0x00, 0x25, 0x00, 0x00, 0x00, 0x00};
//    stream = std::istrstream(reinterpret_cast<const char*>(data.data()), data.size());
//    transporter.receive(stream);

    if (listener.m_bool)
    {
        std::cout << "\nrmove from parent\n";
        std::cout << "----------------------------\n";

        // sets dirty
        listener.m_bool->removeFromParent();
        client.dump();
    }
    else
    {
        std::cout << "no bool parameter\n";
    }
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
    param.update(param2.getShared());

    // adjust param2 and update
    std::cout << "-------\n";
    param2.setDescription("parameter description");
    param.update(param2.getShared());

    // remove callback 1 and update again
    std::cout << "-------\n";
    std::cout << "remove cb1\n";
    param.removeUpdatedCb(cb1);
    param.update(param2.getShared());

    std::cout << "\n\n";

    param.removeUpdatedCb(cb2);
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
    param.update(param2.getShared());

    param.removeValueUpdatedCb(cbl);

    param2.setValue(false);
    param.update(param2.getShared());

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
    param.update(param2.getShared());

    //
    std::cout << "---------\n";
    param.removeValueUpdatedCb(cb1);
    param2.setValue(false);
    param.update(param2.getShared());

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

    auto sp = server.createInt8Parameter("i8", group);
    sp->setValue(10);

    auto bang = server.createBangParameter("bang", group);
    bang->setDescription("desc");


    server.update();

    serverTransporter.testcallall("blubb");

    std::cout << "\n\n";
}


void testInit()
{
    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    ParameterServer server;
    DummyServerTransporter serverTransporter;

    server.addTransporter(serverTransporter);

    auto sp = server.createInt8Parameter("i8");
    auto bang = server.createBangParameter("bang");


    // trigger init
    rcp::Packet packet(COMMAND_INITIALIZE);
    StringStreamWriter aWriter;
    packet.write(aWriter, true);

    serverTransporter.testcallall(aWriter.getBuffer());

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

void _parseStream(std::istream& stream)
{
    if (!stream.good())
    {
        return;
    }

    Option<Packet> packet_option = Packet::parse(stream);

    if (!packet_option.hasValue()) {
        std::cout << "error: no packet!\n";
        return;
    }

    Packet& the_packet = packet_option.value();

    std::cout << "packet command: " << the_packet.getCommand() << "\n";

    if (the_packet.hasTimestamp()) {
        std::cout << "packet timestamp: " << the_packet.getTimestamp() << "\n";
    }


    switch (the_packet.getCommand()) {

    case COMMAND_INVALID:
    case COMMAND_MAX_:
    case COMMAND_DISCOVER:
    case COMMAND_INITIALIZE:
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

    case COMMAND_UPDATEVALUE:
    case COMMAND_UPDATE:
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
//                std::cout << "boolean default: " << p->getDefault() << "\n";
//                std::cout << "boolean value: " << p->getValue() << "\n";

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

    case COMMAND_REMOVE:
    {
        if (!the_packet.hasData()) {
            std::cout << "remove packet does not contain data\n";
            return;
        }

        // assume this is a id-data
        IdDataPtr data = std::dynamic_pointer_cast<IdData>(the_packet.getData());
        if (data) {
            std::cout << "id: " << data->getId();
        }

        break;
    }
    }
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


void testCustomParameter() {

    std::cout << "**** " << __FUNCTION__ << " ****\n\n";

    char uuid[16];
    for (char i=0; i<16; i++)
    {
        uuid[int(i)] = i + 65; // A
    }

    std::vector<char> config;
    config.push_back(5);
    config.push_back(6);
    config.push_back(7);

    uint32_t v = 10;
    std::vector<char> value(sizeof(uint32_t));
    memcpy(value.data(), &v, sizeof(uint32_t));

    uint32_t dv = 5;
    std::vector<char> default_value(sizeof(uint32_t));
    memcpy(default_value.data(), &dv, sizeof(uint32_t));

    CustomParameterPtr param = CustomParameter::create(1, sizeof(uint32_t));
    param->getDefaultTypeDefinition().setUuid(uuid, 16);
    param->setConfig(config);
    param->setValue(value);
    param->setDefault(default_value);

    param->dump();

    // serialize packet
    Packet packet(COMMAND_UPDATE);
    packet.setData(param);

    StringStreamWriter writer;
    packet.write(writer, true);

    std::cout << "CustomParameter:\n";
    writer.dump();

    // parse packet
    _parseStream(writer.getBuffer());

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


void testRemovePacket() {

    // serialize
    Packet packet(COMMAND_REMOVE);
    IdDataPtr data = IdData::create(576);
    packet.setData(data);

    StringStreamWriter writer;
    packet.write(writer, true);

    std::cout << "infodata packet:\n";
    writer.dump();
}

void testUpdateValuePacket() {

    // serialize
    Packet packet(COMMAND_UPDATEVALUE);

    ParameterServer server;
    rcp::BooleanParameterPtr bool1 = server.createBooleanParameter("bool 1");
    bool1->setValue(true);

    IdDataPtr data = IdData::create(576);
    packet.setData(data);

    StringStreamWriter writer;
    packet.write(writer, true);

    std::cout << "infodata packet:\n";
    writer.dump();
}


void testBoolParam()
{
    ParameterServer server;
//    ParameterManager manager;

    auto boolParam = server.createBooleanParameter("bool");

    std::cout << "bool value:" << boolParam->getValue() << "\n";
}

void testFloatParam()
{
//    ParameterServer server;
//    auto boolParam = server.createBooleanParameter("bool");
    Float32ParameterPtr param = Float32Parameter::create(1);

    std::cout << "has min: " << param->getDefaultTypeDefinition().hasMinimum() << "\n";
    std::cout << "has max: " << param->getDefaultTypeDefinition().hasMaximum() << "\n";
    std::cout << "has default: " << param->getDefaultTypeDefinition().hasDefault() << "\n";
}


void testVector2f()
{
    rcp::Vector2f v1;
    assert(v1.x() == 0);
    assert(v1.y() == 0);

    // test set
    v1.set(1, 2);
    assert(v1.x() == 1);
    assert(v1.y() == 2);

    // test copy
    rcp::Vector2f v2(v1);
    assert(v1 == v2);
    assert(!(v1 != v2));


    v2.setX(11);

    // test assignment
    rcp::Vector2f v3;    
    v3 = v2;
    assert(v3 == v2);
}

void testVector2fParameter()
{
    Vector2F32ParameterPtr param = Vector2F32Parameter::create(1);

    param->getDefaultTypeDefinition().setMinimum(Vector2f(-1, -1));
    param->getDefaultTypeDefinition().setMaximum(Vector2f(100, 200));
    param->getDefaultTypeDefinition().setDefault(Vector2f(10, 10));
    param->getDefaultTypeDefinition().setMultipleof(Vector2f(0.5, 0.5));
    param->getDefaultTypeDefinition().setUnit("%");
    param->getDefaultTypeDefinition().setScale(NUMBER_SCALE_LINEAR);

    param->setValue(Vector2f(10, 11));


    Packet packet(COMMAND_UPDATE);
    packet.setData(param);

    StringStreamWriter writer;
    packet.write(writer, true);

    writer.dump();

    _parseStream(writer.getBuffer());
}

void testVector2iParameter()
{
    Vector2I32ParameterPtr param = Vector2I32Parameter::create(1);

    param->getDefaultTypeDefinition().setMinimum(Vector2i(-1, -1));
    param->getDefaultTypeDefinition().setMaximum(Vector2i(100, 200));
    param->getDefaultTypeDefinition().setDefault(Vector2i(10, 10));
    param->getDefaultTypeDefinition().setMultipleof(Vector2i(2, 2));
    param->setValue(Vector2i(10, 11));


    Packet packet(COMMAND_UPDATE);
    packet.setData(param);

    StringStreamWriter writer;
    packet.write(writer, true);

    writer.dump();

    _parseStream(writer.getBuffer());
}


void testVector3f()
{
    rcp::Vector3f v1;
    assert(v1.x() == 0);
    assert(v1.y() == 0);

    // test set
    v1.set(1, 2, 3);
    assert(v1.x() == 1);
    assert(v1.y() == 2);
    assert(v1.z() == 3);

    // test copy
    rcp::Vector3f v2(v1);
    assert(v1 == v2);
    assert(!(v1 != v2));


    v2.setX(11);

    // test assignment
    rcp::Vector3f v3;
    v3 = v2;
    assert(v3 == v2);
}

void testVector3fParameter()
{
    Vector3F32ParameterPtr param = Vector3F32Parameter::create(1);

    param->getDefaultTypeDefinition().setMinimum(Vector3f(-1, -1, -1));
    param->getDefaultTypeDefinition().setMaximum(Vector3f(100, 200, 120));
    param->getDefaultTypeDefinition().setDefault(Vector3f(10, 10, 12));
    param->getDefaultTypeDefinition().setMultipleof(Vector3f(0.5, 0.5, 0.5));
    param->setValue(Vector3f(10, 11, 12));


    Packet packet(COMMAND_UPDATE);
    packet.setData(param);

    StringStreamWriter writer;
    packet.write(writer, true);

    writer.dump();

    _parseStream(writer.getBuffer());
}

void testVector4f()
{
    rcp::Vector4f v1;
    assert(v1.x() == 0);
    assert(v1.y() == 0);

    // test set
    v1.set(1, 2, 3, 4);
    assert(v1.x() == 1);
    assert(v1.y() == 2);
    assert(v1.z() == 3);
    assert(v1.w() == 4);

    // test copy
    rcp::Vector4f v2(v1);
    assert(v1 == v2);
    assert(!(v1 != v2));


    v2.setX(11);

    // test assignment
    rcp::Vector4f v3;
    v3 = v2;
    assert(v3 == v2);
}

void testVector4fParameter()
{
    Vector4F32ParameterPtr param = Vector4F32Parameter::create(1);

    param->getDefaultTypeDefinition().setMinimum(Vector4f(-1, -1, -1, -1));
    param->getDefaultTypeDefinition().setMaximum(Vector4f(100, 200, 120, 110));
    param->getDefaultTypeDefinition().setDefault(Vector4f(10, 10, 12, 11));
    param->getDefaultTypeDefinition().setMultipleof(Vector4f(0.5, 0.5, 0.5, 0.5));
    param->setValue(Vector4f(10, 11, 12, 13));


    Packet packet(COMMAND_UPDATE);
    packet.setData(param);

    StringStreamWriter writer;
    packet.write(writer, true);

    writer.dump();

    _parseStream(writer.getBuffer());
}


// test threading
static inline std::string nowString()
{
    std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string now = std::ctime(&end_time);
    std::replace(now.begin(), now.end(), '\n', ' ');

    return now;
}

void _threaded_func(Float32Parameter param)
{
    std::cout << nowString() << "_threaded_func - on thread:" << std::this_thread::get_id() << " - label: " << param.getLabel() << "\n";

    param.setValue(13);
    param.setMaximum(10);

    std::cout << nowString() << "_threaded_func on thread done:\n";
}


void testThreading()
{
    std::cout << nowString() << "testThreading - on thread:" << std::this_thread::get_id() << "\n";

    Float32ParameterPtr param = Float32Parameter::create(1);

    // start thread
    // copy param to make sure we use the same mutex
    std::thread t(_threaded_func, *param.get());

    param->setLabel("test");

    t.join();

    std::cout << nowString() << "testThreading done\n";
}


//-------------------------------
//-------------------------------
int main(int /*argc*/, char const */*argv*/[])
{
    testInit();
    return 0;

    testThreading();
    return 0;

    testCustomParameter();
    return 0;

    testVector2f();
    testVector2fParameter();
    testVector2iParameter();
    testVector3f();
    testVector3fParameter();
    testVector4f();
    testVector4fParameter();
    return 0;

//    std::vector<char> data = {0x04, 0x12, 0x00, 0x09, 0x10, 0x00, 0x21, 0x61, 0x6e, 0x79, 0x04, 0x62, 0x6f, 0x6f, 0x6c, 0x00, 0x25, 0x00, 0x07, 0x00, 0x00};
//    parseData(data);
//    return 0;

////    testFloatParam();
//    parseData("../RCP/example_data/packet_update_float32.rcp");
//    return 0;

//    parseData("../RCP/example_data/packet_update_bool.rcp");
////    testBoolParam();
//    return 0;

//    testEnumParam();
//    return 0;

//    parseData("../RCP/example_data/packet_updatevalue_s8.rcp");
//    parseData("../RCP/example_data/packet_updatevalue_s32.rcp");
//    parseData("../RCP/example_data/packet_updatevalue_string.rcp");
//    return 0;


//    parseData("../RCP/example_data/packet_remove.rcp");
//    testRemovePacket();
//    return 0;

//    parseData("../RCP/example_data/packet_bool_no_user.rcp");
//    return  0;


    testClientParents();
//    testHierarchy();
    return 0;

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
    parseData("../RCP/example_data/packet_bool_userdata.rcp");
    parseData("../RCP/example_data/packet_update_bool.rcp");
    parseData("../RCP/example_data/packet_update_boolarray_no_user.rcp");
    parseData("../RCP/example_data/packet_enum.rcp");
    parseData("../RCP/example_data/packet_initialize.rcp");
    parseData("../RCP/example_data/packet_initialize_id_data.rcp");

    parseData("../RCP/example_data/packet_lstr_no_user.rcp");
    parseData("../RCP/example_data/packet_range.rcp");
    parseData("../RCP/example_data/packet_s8_no_user.rcp");
    parseData("../RCP/example_data/packet_string_default.rcp");
    parseData("../RCP/example_data/packet_u32_no_user.rcp");
    parseData("../RCP/example_data/packet_update_u32.rcp");
    parseData("../RCP/example_data/packet_uri.rcp");


    return 0;
}

