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

#ifndef PARAMETERCLIENT_H
#define PARAMETERCLIENT_H

#include "packet.h"
#include "clienttransporter.h"
#include "parametermanager.h"
#include "rcp_error_listener.h"

namespace rcp {

    class ParameterClientListener
    {
    public:
        virtual void parameterAdded(ParameterPtr parameter) = 0;
        virtual void parameterRemoved(ParameterPtr parameter) = 0;
    };


    class ParameterClient : public ClientTransporterListener
    {
    public:
        ParameterClient(ClientTransporter& transporter);
        virtual ~ParameterClient();

        void dispose();
        void connect(const std::string& host, const int port);
        void disconnect();

        void initialize(); // tries to send an init-command
        void update(); // update all changes

        // connect to events
        void addParameterAddedCb(ParameterClientListener* c, void(ParameterClientListener::* func)(ParameterPtr parameter)) {
            parameter_added_cb[c] = func;
        }
        void removeParameterAddedCb(ParameterClientListener* c) {
            parameter_added_cb.erase(c);
        }

        void addParameterRemovedCb(ParameterClientListener* c, void(ParameterClientListener::* func)(ParameterPtr parameter)) {
            parameter_removed_cb[c] = func;
        }
        void removeParameterRemovedCb(ParameterClientListener* c) {
            parameter_removed_cb.erase(c);
        }

        void addParsingErrorCb(ParsingErrorListener* c, void(ParsingErrorListener::* func)()) {
            parsing_error_cb[c] = func;
        }
        void removeParsingErrorCb(ParsingErrorListener* c) {
            parsing_error_cb.erase(c);
        }


        void setApplicationId(const std::string& appid) {
            m_applicationId = appid;
        }
        std::string getApplicationId() {
            return m_applicationId;
        }

        GroupParameterPtr rootGroup() const {
            return m_parameterManager->rootGroup();
        }

        std::string getServerVersion() const
        {
            return m_serverVersion;
        }
        std::string getServerApplicationId() const
        {
            return m_serverApplicationId;
        }

    public:
        // interface ClientTransporterListener
        virtual void connected();
        virtual void disconnected();
        virtual void received(std::istream& data);

    protected:
        std::shared_ptr<ParameterManager> m_parameterManager;
        ClientTransporter& m_transporter;

    private:
        void _update(Packet& packet);
        void _remove(Packet& packet);
        void _version(Packet& packet);

        // Events:
        std::map<ParameterClientListener*, void(ParameterClientListener::*)(ParameterPtr parameter)> parameter_added_cb;
        std::map<ParameterClientListener*, void(ParameterClientListener::*)(ParameterPtr parameter)> parameter_removed_cb;
        std::map<ParsingErrorListener*, void(ParsingErrorListener::*)()> parsing_error_cb;
//        onError(Exception ex);
//        statusChanged(Status status, String message);

        std::string m_applicationId;

        std::string m_serverApplicationId;
        std::string m_serverVersion;
    };

}


#endif // PARAMETERCLIENT_H
