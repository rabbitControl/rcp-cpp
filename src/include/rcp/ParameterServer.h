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

#ifndef RCP_PARAMETERSERVER_H
#define RCP_PARAMETERSERVER_H

#include "ParameterManager.h"
#include "ServerTransporter.h"
#include "StringStreamWriter.h"

namespace rcp {

class ParameterServerListener
{
public:
    virtual void clientInfoReceived(const std::string& /*applicationId*/, const std::string& /*version*/) {};
};


class ParameterServer
    : public ServerTransporterListener
{
public:
    ParameterServer(const IGroupParameterPtr& root = nullptr);
    ParameterServer(ServerTransporter& transporter, const IGroupParameterPtr& root = nullptr);
    ~ParameterServer();


    bool addTransporter(ServerTransporter& transporter);
    bool removeTransporter(ServerTransporter& transporter);
    int getConnectionCount();

    // listener
    void addListener(ParameterServerListener* listener);
    void removeListener(ParameterServerListener* listener);


    virtual bool update() = 0;
    void sendData(StringStreamWriter& writer);


    void setApplicationId(const std::string& appid);
    std::string getApplicationId();

    virtual void clientConnected(void* client) {}
    virtual void clientDisconnected(void* client) {}


protected:
    bool _update(const IParameterPtr &param)
    {
        if (param)
        {
            auto chached_param = m_manager->getParameter(param->getId());

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

        }
        else
        {
            // no parameter?
        }

        return false;
    }


protected:
    std::string m_applicationId;

    std::vector<std::reference_wrapper<ServerTransporter> > transporterList;
    std::vector<ParameterServerListener*> m_listener;

    ParameterManagerPtr m_manager;
};


}

#endif // PARAMETERSERVER_H
