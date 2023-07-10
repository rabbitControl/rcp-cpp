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
#include "parameterclient.h"

#include "stringstreamwriter.h"
#include "streamwriter.h"
#include "rcp.h"

namespace rcp {

    ParameterClient::ParameterClient(ClientTransporter& transporter)
        : m_parameterManager(std::make_shared<rcp::ParameterManager>())
        , m_transporter(transporter)
    {
        transporter.addConnectedCb(this, &ClientTransporterListener::connected);
        transporter.addDisconnectedCb(this, &ClientTransporterListener::disconnected);
        transporter.addReceivedCb(this, &ClientTransporterListener::received);
    }

    ParameterClient::~ParameterClient() {
        dispose();
    }

    void ParameterClient::dispose() {
        disconnect();
    }

    void ParameterClient::connect(const std::string& host, const int port) {
        m_transporter.connect(host, port);
    }

    void ParameterClient::disconnect() {
        m_transporter.disconnect();
    }

    void ParameterClient::initialize() {

        char data[2];
        data[0] = 0x02;
        data[1] = 0x00;
        m_transporter.send(data, 2);
    }

    // sending all dirty parameter to server!
    void ParameterClient::update() {

        if (!m_transporter.isConnected()) {
            return;
        }
		
		// protect lists to be used from multiple threads
		m_parameterManager->lock();

        // send updates
        for (auto& p : m_parameterManager->dirtyParameter) {

            command_t cmd = COMMAND_UPDATE;

            if (p.second->onlyValueChanged())
            {
                cmd = COMMAND_UPDATEVALUE;
            }

            Packet packet(cmd, p.second);

            // serialize
            StringStreamWriter writer;
            packet.write(writer, false);

            m_transporter.send(writer.getBuffer());
        }
        m_parameterManager->dirtyParameter.clear();

		m_parameterManager->unlock();
    }

    // interface ClientTransporterListener
    void ParameterClient::connected()
    {
        // request version
        char data[2];
        data[0] = 0x01;
        data[1] = 0x00;
        m_transporter.send(data, 2);
    }

    void ParameterClient::disconnected()
    {
        m_parameterManager->clear();
    }

    void ParameterClient::received(std::istream& data)
    {
        auto packet = rcp::Packet::parse(data, m_parameterManager);
        if (packet.hasValue()) {

            rcp::Packet& the_packet = packet.value();
            switch (the_packet.getCommand()) {

            case COMMAND_INITIALIZE:
                // error
                std::cerr << "invalid command 'initialize' on client\n";
                break;

            case COMMAND_UPDATE:
            case COMMAND_UPDATEVALUE:
                _update(the_packet);
                break;

            case COMMAND_INFO:
                _version(the_packet);
                break;

            case COMMAND_DISCOVER:
                // not implemented
                std::cerr << "invalid command 'discover' on client\n";
                break;

            case COMMAND_REMOVE:
                _remove(the_packet);
                break;

            case COMMAND_INVALID:
            case COMMAND_MAX_:
                std::cerr << "got invalid command!\n";
                break;
            }
        }
        else
        {
            // parsing error??
            for (const auto& kv : parsing_error_cb) {
                (kv.first->*kv.second)();
            }
        }
    }

    void ParameterClient::_version(Packet& packet) {

        if (packet.hasData()) {
            // log info data
            InfoDataPtr info_data = std::dynamic_pointer_cast<InfoData>(packet.getData());
            if (info_data)
            {
                m_serverApplicationId = info_data->getApplicationId();
                m_serverVersion = info_data->getVersion();

                for (auto listener : m_listener)
                {
                    listener->serverInfoReceived(m_serverApplicationId, m_serverVersion);
                }

                // TODO: compare RCP version
                // only send initialize after compatibility-check
                bool version_ok = true;

                if (!version_ok)
                {
                    // TODO: disconnect from server
                }

                if (!initializeSent &&
                        version_ok)
                {
                    // send initialize command
                    initialize();
                    initializeSent = true;
                }
            }
        } else {
            // no data, respond with version
            WriteablePtr version = InfoData::create(RCP_SPECIFICATION_VERSION, m_applicationId);
            Packet resp_packet(COMMAND_INFO, version);
            StringStreamWriter writer;
            resp_packet.write(writer, false);
            m_transporter.send(writer.getBuffer());
        }
    }


    // private functions
    // receiving dirty parameter from server!
    void ParameterClient::_update(rcp::Packet& packet) {

        if (!packet.hasData()) {
            std::cerr << "_update: packet has no data\n";
            return;
        }

        // assume this is a parameter
        rcp::ParameterPtr param = std::dynamic_pointer_cast<rcp::IParameter>(packet.getData());
        if (param)
        {
            rcp::ParameterPtr chached_param = m_parameterManager->getParameter(param->getId());

            if (chached_param)
            {
                // got it... update it
                chached_param->update(param);

            } else {

                // parameter not in cache, add it
                m_parameterManager->_addParameter(param);

                // call parameter added callbacks
                for (ParameterClientListener* listener : m_listener)
                {
                    listener->parameterAdded(param);
                }
            }
            return;

        } else {
            std::cerr << "data not a parameter\n";
        }

    }

    void ParameterClient::_remove(Packet& packet) {

        if (!packet.hasData()) {
            std::cerr << "_remove: packet has no data\n";
            return;
        }

        // assume this is id-data (rcp > 0.0.0)
        IdDataPtr id_data = std::dynamic_pointer_cast<IdData>(packet.getData());
        if (id_data) {

            std::cout << "remove param: " << id_data->getId() << "\n";

            rcp::ParameterPtr cached_param = m_parameterManager->getParameter(id_data->getId());
            if (cached_param)
            {
                // parameter is in list, remove it
                std::cout << "removing exisiting parameter: " << id_data->getId() << "\n";

                // call parameter removed callbacks
                for (ParameterClientListener* listener : m_listener)
                {
                    listener->parameterRemoved(cached_param);
                }

                // remove it (direct)
                m_parameterManager->removeParameterDirect(cached_param);

            } else {
                std::cout << "parameter not in list!: " << id_data->getId() << "\n";
            }
            return;

        } else {
            std::cerr << "data not a parameter\n";
        }
    }

    void ParameterClient::addListener(ParameterClientListener* listener)
    {
        if (std::find(m_listener.begin(), m_listener.end(), listener) == m_listener.end())
        {
            // not found - add it to list
            m_listener.push_back(listener);
        }
    }

    void ParameterClient::removeListener(ParameterClientListener* listener)
    {
        auto it = std::find(m_listener.begin(), m_listener.end(), listener);
        if (it != m_listener.end())
        {
            // rmove listener
            m_listener.erase(it);
        }
    }

} // namespace rcp

