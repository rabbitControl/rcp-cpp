/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2023
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#include "parameterserver.h"

#include "rcp.h"
#include "streamwriter.h"
#include "infodata.h"

namespace rcp {

    ParameterServer::ParameterServer()
        : m_parameterManager(std::make_shared<ParameterManager>())
        , m_applicationId("cpp-server")
    {
    }

    ParameterServer::ParameterServer(ServerTransporter& transporter)
        : m_parameterManager(std::make_shared<ParameterManager>())
        , m_applicationId("cpp-server")
    {
        addTransporter(transporter);
    }

    ParameterServer::~ParameterServer() {
        dispose();
    }

    void ParameterServer::dispose() {
        // TODO
        clear();
    }

    void ParameterServer::clear() {
        m_parameterManager->clear();
    }


    void ParameterServer::received(std::istream& data, ServerTransporter& transporter, void* id)
    {
        // parse data
        Option<Packet> packet_option = Packet::parse(data, m_parameterManager);

        if (packet_option.hasValue())
        {
            // got a packet
            Packet& the_packet = packet_option.value();

            switch (the_packet.getCommand())
            {
            case COMMAND_INITIALIZE:
                _init(transporter, id);
                break;

            case COMMAND_UPDATEVALUE:
            case COMMAND_UPDATE:
                if (_update(the_packet, transporter, id)) {
                    // send data to all clients
                    for (auto& transporter : transporterList) {
                        transporter.get().sendToAll(data, id);
                    }
                }
                break;

            case COMMAND_INFO:
                // handle version-packet
                _version(the_packet, transporter, id);
                break;

            case COMMAND_DISCOVER:
                // not implemented
                break;

            case COMMAND_REMOVE:
                // error!
                break;

            case COMMAND_INVALID:
            case COMMAND_MAX_:
                break;
            }
        }
        else
        {
            for (const auto& kv : parsing_error_cb) {
                (kv.first->*kv.second)();
            }
        }
    }

    bool ParameterServer::addTransporter(ServerTransporter& transporter) {

        for(auto it = transporterList.begin(); it != transporterList.end(); it++ )    {
            if (&transporter == &(*it).get()) {
                // allready contained
                return false;
            }
        }

        // register as listener
        transporter.addReceivedCb(this, &ServerTransporterReceiver::received);

        transporterList.push_back(transporter);

        return true;
    }

    bool ParameterServer::removeTransporter(ServerTransporter& transporter) {

        for(auto it = transporterList.begin(); it != transporterList.end(); it++ )    {
            if (&transporter == &(*it).get()) {
                // contained
                it->get().removeReceivedCb(this);
                transporterList.erase(it);
                return true;
            }
        }

        return false;
    }

    int ParameterServer::getConnectionCount() {

        int count = 0;

        for (const auto& t : transporterList) {
            count += t.get().getConnectionCount();
        }

        return count;
    }


    bool ParameterServer::update() {

        if (transporterList.size() == 0) {
            return false;
        }
		
		// protect lists to be used from multiple threads
        m_parameterManager->lock();

        // send removes
        for (auto& p : m_parameterManager->removedParameter)
        {
            WriteablePtr id_data = IdData::create(p.second->getId());
            Packet packet(COMMAND_REMOVE, id_data);
            sendPacket(packet);
        }
        m_parameterManager->removedParameter.clear();

        // send updates
        for (auto& p : m_parameterManager->dirtyParameter) {

            // TODO send COMMAND_UPDATEVALUE
            command_t cmd = COMMAND_UPDATE;

            if (p.second->onlyValueChanged())
            {
                cmd = COMMAND_UPDATEVALUE;
            }

            Packet packet(cmd, p.second);
            sendPacket(packet);
        }
        m_parameterManager->dirtyParameter.clear();

		// unlock mutex
        m_parameterManager->unlock();
		
        return false;
    }

    void ParameterServer::sendPacket(Packet& packet, void *id) {

        // serialize
        StringStreamWriter writer;
        packet.write(writer, false);

        for (auto& transporterW : transporterList) {
            transporterW.get().sendToAll(writer.getBuffer(), id);
        }
    }


    void ParameterServer::_sendParameterFull(ParameterPtr& parameter, ServerTransporter& transporter, void *id) {

        Packet packet(COMMAND_UPDATE);
        packet.setData(parameter);

        // serialize
        StringStreamWriter aWriter;
        packet.write(aWriter, true);

        transporter.sendToOne(aWriter.getBuffer(), id);

        if (parameter->getTypeDefinition().getDatatype() == DATATYPE_GROUP) {

            std::shared_ptr<GroupParameter> group_param = std::dynamic_pointer_cast<GroupParameter>(parameter);
            if (group_param) {
                for (auto& child : group_param->children()) {
                    _sendParameterFull(child.second, transporter, id);
                }
            }
        }
    }


    void ParameterServer::_init(ServerTransporter& transporter, void *id) {

        for (auto& child : m_parameterManager->rootGroup()->children()) {
            _sendParameterFull(child.second, transporter, id);
        }
    }

    bool ParameterServer::_update(Packet& packet, ServerTransporter& transporter, void *id) {

        if (!packet.hasData()) {
            std::cerr << "_update: packet has no data\n";
            return false;
        }

        // assume this is a parameter!!
        ParameterPtr param = std::dynamic_pointer_cast<IParameter>(packet.getData());
        if (param)
        {
            ParameterPtr chached_param = m_parameterManager->getParameter(param->getId());

            if (chached_param)
            {
                // got it... update it
                chached_param->update(param);

                // call updateParameter callbacks
            }
            else
            {
                // nope - we don't add parameter on a server
            }

            return true;

        } else {
            std::cerr << "data not a parameter\n";
        }

        return false;
    }

    void ParameterServer::_version(Packet& packet, ServerTransporter& transporter, void *id) {

        // got client
        if (packet.hasData()) {
            // log info data
            InfoDataPtr info_data = std::dynamic_pointer_cast<InfoData>(packet.getData());
            if (info_data) {
                std::cout << "version: " << info_data->getVersion() << std::endl;
                std::cout << "applicationid: " << info_data->getApplicationId() << std::endl;
            }
        } else {
            // no data, respond with version
            WriteablePtr version = InfoData::create(RCP_SPECIFICATION_VERSION, m_applicationId);
            Packet resp_packet(COMMAND_INFO, version);
            StringStreamWriter writer;
            resp_packet.write(writer, false);
            transporter.sendToOne(writer.getBuffer(), id);

            // ask for version
            StringStreamWriter writer1;
            Packet req_info_packet(COMMAND_INFO);
            req_info_packet.write(writer1, false);
            transporter.sendToOne(writer1.getBuffer(), id);

        }

    }

}
