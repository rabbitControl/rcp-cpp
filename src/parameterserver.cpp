#include "ParameterServer.h"

#include "StringStreamWriter.h"

namespace rcp {

ParameterServer::ParameterServer(const IGroupParameterPtr& root)
    : m_applicationId("cpp-server")
    , m_manager(ParameterManager::create(root))
{
}

ParameterServer::ParameterServer(ServerTransporter& transporter, const IGroupParameterPtr& root)
    : m_applicationId("cpp-server")
    , m_manager(ParameterManager::create(root))
{
    addTransporter(transporter);
}

ParameterServer::~ParameterServer()
{
    for (auto it = transporterList.begin(); it != transporterList.end(); it++)
    {
        it->get().removeListener(this);
    }
}

bool ParameterServer::addTransporter(ServerTransporter& transporter)
{
    for (const auto& t : transporterList)
    {
        if (&transporter == &t.get())
        {
            // already contained
            return false;
        }
    }

    // register as listener
    transporter.addListener(this);
    transporterList.push_back(transporter);

    return true;
}

bool ParameterServer::removeTransporter(ServerTransporter& transporter) {

    for (auto it = transporterList.begin(); it != transporterList.end(); it++ )
    {
        if (&transporter == &(*it).get())
        {
            // contained
            it->get().removeListener(this);
            transporterList.erase(it);
            return true;
        }
    }

    return false;
}

int ParameterServer::getConnectionCount()
{
    int count = 0;

    for (const auto& t : transporterList)
    {
        count += t.get().getConnectionCount();
    }

    return count;
}


// listener
void ParameterServer::addListener(ParameterServerListener* listener)
{
    if (std::find(m_listener.begin(), m_listener.end(), listener) == m_listener.end())
    {
        // not found - add it to list
        m_listener.push_back(listener);
    }
}

void ParameterServer::removeListener(ParameterServerListener* listener)
{
    auto it = std::find(m_listener.begin(), m_listener.end(), listener);
    if (it != m_listener.end())
    {
        // rmove listener
        m_listener.erase(it);
    }
}

void ParameterServer::sendData(StringStreamWriter &writer)
{
    for (auto& transporterW : transporterList)
    {
        transporterW.get().sendToAll(writer.getBuffer(), nullptr);
    }
}




void ParameterServer::setApplicationId(const std::string &appid) {
    m_applicationId = appid;
}

std::string ParameterServer::getApplicationId() {
    return m_applicationId;
}


}

