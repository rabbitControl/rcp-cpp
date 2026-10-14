/*
********************************************************************
* RabbitControl - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of RabbitControl for c++.
*
* Written by Ingo Randolf, 2018-2026
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCP_V10_PARAMETERCLIENT_H
#define RCP_V10_PARAMETERCLIENT_H

#include "../ClientTransporter.h"
#include "../ParameterClient.h"

#include "InfoData.h"
#include "ParameterClientState.h"

namespace rcp {
namespace v10 {

class ParameterClient
    : public rcp::ParameterClient
{
public:
    ParameterClient(ClientTransporter& transporter);

    std::string getServerVersion() const;
    std::string getServerApplicationId() const;

    void setApplicationVersion(const std::string& appid);
    std::string getApplicationVersion() const;

    void setVersionMismatchCb(const std::function<void(const InfoData&)>& cb);

    RcpVersion getRcpVersion() const;
    RcpVersion getHandshakeVersion() const;

public:
    // rcp::ParameterClient
    virtual void update() override;

    // ClientTransporterListener
    virtual void connected() override;
private:
    // ClientTransporterListener
    virtual void disconnected() override;
    virtual void received(std::istream& data) override;

private:
    void _init(const std::vector<int32_t>& idData);
    void _version(const InfoData& info);
    void _remove(const std::vector<int32_t>& info);
    void _update(const std::vector<IParameterPtr>& parameter, bool updateValue);
    void _sendInitialize();

    void errorAndClose(const std::string& error);
    void setState(ClientState state);

private:
    bool m_initializeSent{false};

    std::string m_applicationVersion;

    // server
    std::string m_serverApplicationId;
    std::string m_serverApplicationVersion;

    ClientState m_state{ClientState::Unknown};

    RcpVersion m_rcpVersion;
    RcpVersion m_handshakeVersion;

    uint32_t m_expectedParameterCount{0};

    std::function<void(const InfoData& serverInfo)> m_versionMismatchCb;
};


}
}


#endif // PARAMETERCLIENT_H
