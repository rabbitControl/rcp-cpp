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

#ifndef RCP_SERVERTRANSPORTER_H
#define RCP_SERVERTRANSPORTER_H

#include <istream>

namespace rcp {

class ServerTransporter;

class ServerTransporterListener
{
public:
    virtual void received(std::istream& data, ServerTransporter& transporter, void* id) = 0;
    virtual void connected(void* client) = 0;
    virtual void disconnected(void* client) = 0;
};


class ServerTransporter
{
    using ClientCb = std::function<void(void*)>;

public:
    virtual void bind(int port) = 0;
    virtual void unbind() = 0;

    virtual void sendToOne(std::istream& data, void* id) = 0;
    virtual void sendToAll(std::istream& data, void* excludeId) = 0;

    virtual int getConnectionCount() = 0;

    virtual void closeClient(void* id) = 0;

    void addListener(ServerTransporterListener* listener)
    {
        if (std::find(m_listener.begin(), m_listener.end(), listener) == m_listener.end())
        {
            m_listener.push_back(listener);
        }
    }

    void removeListener(ServerTransporterListener* listener)
    {
        auto it = std::find(m_listener.begin(), m_listener.end(), listener);
        if (it != m_listener.end())
        {
            m_listener.erase(it);
        }
    }

protected:
    void _received(std::istream& in, void* client)
    {
        for (auto& listener : m_listener)
        {
            listener->received(in, *this, client);
        }
    }

    void _connected(void* client)
    {
        for (auto& l : m_listener)
        {
            l->connected(client);
        }
    }

    void _disconnected(void* client)
    {
        for (auto& l : m_listener)
        {
            l->disconnected(client);
        }
    }

    std::vector<ServerTransporterListener*> m_listener;
};
}


#endif // SERVERTRANSPORTER_H
