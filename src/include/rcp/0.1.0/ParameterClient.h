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

#ifndef RCP_V010_PARAMETERCLIENT_H
#define RCP_V010_PARAMETERCLIENT_H

#include "../ClientTransporter.h"
#include "../ParameterClient.h"

#include "IdData.h"
#include "InfoData.h"

namespace rcp {
namespace v010 {

class ParameterClient
    : public rcp::ParameterClient
{
public:
    ParameterClient(ClientTransporter& transporter,
                    const std::function<void()>& v10Callback = []{});

    std::string getServerVersion() const;
    std::string getServerApplicationId() const;

public:
    // rcp::ParameterClient
    virtual void update() override;

private:
    // ClientTransporterListener
    virtual void received(std::istream& data) override;

private:
    void _init(const IdDataPtr& idData);
    void _version(const InfoDataPtr& info);
    void _remove(const IdDataPtr& info);
    void _sendInitialize();

private:
    bool m_initializeSent{false};

    std::function<void()> m_v10Callback;

    // server
    std::string m_serverApplicationId;
    std::string m_serverVersion;
};


}
}


#endif // PARAMETERCLIENT_H
