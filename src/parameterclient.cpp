#include "ParameterClient.h"

#include "0.1.0/ParameterClient.h"
#include "0.1.0/types.h"

#include "1.0/ParameterClient.h"
#include "1.0/types.h"

namespace rcp {

ParameterClient::ParameterClient(ClientTransporter& transporter)
    : m_transporter(transporter)
{
    transporter.addConnectedCb(this, &ClientTransporterListener::connected);
    transporter.addDisconnectedCb(this, &ClientTransporterListener::disconnected);
    transporter.addReceivedCb(this, &ClientTransporterListener::received);
}

ParameterClient::~ParameterClient()
{
    m_transporter.removeConnectedCb(this);
    m_transporter.removeDisconnectedCb(this);
    m_transporter.removeReceivedCb(this);

    if (m_realClient)
    {
        m_realClient.reset();
    }

    if (!m_dontDisconnectOnDestruct)
    {
        disconnect();
    }
}

void ParameterClient::setApplicationId(const std::string &appid)
{
    m_applicationId = appid;
}

std::string ParameterClient::getApplicationId()
{
    return m_applicationId;
}

void ParameterClient::dispose()
{
    if (m_realClient)
    {
        m_realClient->disconnect();
        m_realClient.reset();
    }
    else
    {
        disconnect();
    }
}

void ParameterClient::connect(const std::string& host, const int port)
{
    if (m_realClient)
    {
        m_realClient->connect(host, port);
    }
    else
    {
        m_transporter.connect(host, port);
    }
}

void ParameterClient::disconnect()
{
    if (m_realClient)
    {
        m_realClient->disconnect();
    }
    else
    {
        m_transporter.disconnect();
    }
}

void ParameterClient::addListener(ParameterClientListener *listener)
{
    if (std::find(m_listener.begin(), m_listener.end(), listener) == m_listener.end())
    {
        m_listener.push_back(listener);
    }
}

void ParameterClient::removeListener(ParameterClientListener *listener)
{
    auto it = std::find(m_listener.begin(), m_listener.end(), listener);
    if (it != m_listener.end())
    {
        m_listener.erase(it);
    }
}

IGroupParameterPtr ParameterClient::getRootGroup() const
{
    if (m_manager)
    {
        return m_manager->getRootGroup();
    }

    return nullptr;
}


// ClientTransporterListener

void ParameterClient::connected()
{
    if (m_realClient == nullptr)
    {
        // request version
        char data[2];
        data[0] = 0x01;
        data[1] = 0x00;
        m_transporter.send(data, 2);
    }
}

void ParameterClient::disconnected()
{
    m_manager->_clear();
}

// NOTE: create "real" client if we know which server version we are talking to
// void ParameterClient::received(std::istream& is)
// {
//     if (m_realClient)
//     {
//         std::cerr << "received with real client in place!\n";
//         // error!
//         return;
//     }

//     int result = is.peek();

//     if (result == std::char_traits<char>::eof())
//     {
//         std::cerr << "invalid stream!\n";
//         // throw std::runtime_error("ParameterClient EOF");
//         // disconnect();
//         return;
//     }

//     // mask command with v10 packet-type size 00011111
//     char packet_type_command = result & 0x1F;

//     // v010: commands from 1..6
//     // v10: packet type > 6

//     if (packet_type_command < v010::COMMAND_MAX_)
//     {
//         // v010 packet

//         if (packet_type_command == v010::COMMAND_INFO)
//         {
//             std::cout << "received v010 Info - create real client!\n";

//             m_transporter.removeConnectedCb(this);
//             m_transporter.removeDisconnectedCb(this);
//             m_transporter.removeReceivedCb(this);

//             // create v010 client and pass on data
//             m_realClient = std::make_shared<v010::ParameterClient>(m_transporter);
//             m_realClient->setApplicationId(m_applicationId);

//             //
//             m_realClient->received(is);
//         }
//         else
//         {
//             // this should not happen!
//             throw std::runtime_error("ParameterClient invalid v010 command");
//         }
//     }
//     else
//     {
//         // v10 packet

//         if (packet_type_command == v10::PACKET_TYPES_INFO)
//         {
//             std::cout << "received v10 Info - create real client!\n";

//             m_transporter.removeConnectedCb(this);
//             m_transporter.removeDisconnectedCb(this);
//             m_transporter.removeReceivedCb(this);

//             // create v010 client and pass on data
//             m_realClient = std::make_shared<v10::ParameterClient>(m_transporter);
//             m_realClient->setApplicationId(m_applicationId);
//             m_realClient->setApplicationVersion(m_applicationVersion);

//             //
//             m_realClient->connected();
//             // m_realClient->received(is);
//         }
//         else
//         {
//             throw std::runtime_error("ParameterClient invalid v10 command");
//         }

//     }
// }


void rcp::ParameterClient::sendData(StringStreamWriter &writer)
{
    m_transporter.send(writer.getBuffer());
}

void ParameterClient::setDontDisconnectOnDestruct(bool newDontDisconnectOnDestruct)
{
    m_dontDisconnectOnDestruct = newDontDisconnectOnDestruct;
}

}

