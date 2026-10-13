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

#ifndef RCP_PARAMETERCLIENT_H
#define RCP_PARAMETERCLIENT_H

#include "ClientTransporter.h"
#include "ParameterManager.h"
#include "StringStreamWriter.h"

namespace rcp {


class ParameterClientListener
{
public:
    virtual void serverInfoReceived(const std::string& /*version*/, const std::string& /*applicationId*/, const std::string& applicationVersion = "") {}
    virtual void parameterAdded(IParameterPtr parameter) {}
    virtual void parameterRemoved(IParameterPtr parameter) {}
    virtual void initializeDone() {}
    virtual void parsingError() {}
};



class ParameterClient
    : public ClientTransporterListener
{
public:
    ParameterClient(ClientTransporter& transporter);
    virtual ~ParameterClient();


    void setApplicationId(const std::string& appid);
    std::string getApplicationId();

    virtual void update() = 0;

    void dispose();
    void connect(const std::string& host, const int port);
    void disconnect();

    void addListener(ParameterClientListener* listener);
    void removeListener(ParameterClientListener* listener);

    rcp::IGroupParameterPtr getRootGroup() const;

// private:
    // ClientTransporterListener
    virtual void connected() override;
    virtual void disconnected() override;
    // virtual void received(std::istream& data) override;


    void setDontDisconnectOnDestruct(bool newDontDisconnectOnDestruct);

protected:
    void sendData(StringStreamWriter& writer);

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
                // parameter not in cache, add it
                // this sets the parent
                m_manager->_addParameter(param);

                // NOTE: parameters might not have a parent by now,
                // if waitForParent == true

                // at the client new parameters are all unchanged when added
                param->setAllUnchanged();

                // call parameter added callbacks
                for (auto listener : m_listener)
                {
                    listener->parameterAdded(param);
                }
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
    ClientTransporter& m_transporter;

    std::shared_ptr<ParameterClient> m_realClient{nullptr};
    std::vector<ParameterClientListener*> m_listener;

    std::string m_applicationId;

    ParameterManagerPtr m_manager;

    bool m_dontDisconnectOnDestruct{false};
};

}


#endif // PARAMETERCLIENT_H
