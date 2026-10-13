#include "InfoData.h"
#include "ParameterClient.h"
#include "PacketParser.h"
#include "types.h"
#include "IParameter.h"
#include "GroupParameter.h"
#include "PacketWriter.h"


namespace rcp {
namespace v10 {


ParameterClient::ParameterClient(ClientTransporter& transporter)
    : rcp::ParameterClient(transporter)
{
    m_manager = ParameterManager::create(std::make_shared<GroupParameter>(0, "root"));
}

std::string ParameterClient::getServerVersion() const
{
    return m_serverApplicationVersion;
}

std::string ParameterClient::getServerApplicationId() const
{
    return m_serverApplicationId;
}


void ParameterClient::setApplicationVersion(const std::string& version)
{
    m_applicationVersion = version;
}

std::string ParameterClient::getApplicationVersion() const
{
    return m_applicationVersion;
}

void ParameterClient::update()
{
    if (!m_transporter.isConnected())
    {
        return;
    }

    // protect lists to be used from multiple threads
    m_manager->lock();

    // send updates
    for (auto& p : m_manager->dirtyParameter)
    {
        StringStreamWriter writer;

        PacketWriter::writeUpdate(p.second, writer, false);

        sendData(writer);
    }
    m_manager->dirtyParameter.clear();

    m_manager->unlock();
}

void ParameterClient::connected()
{
    setState(Connected);

    // TODO
    // call connected cb?

    /*

    Client sends a info-packet
    Server receives info-packet
    Server sends info-packet to client
    Server checks if its runtime-version is between client rcp-version and base-version (inclusive)
        Version ok -> wait for Initialization
        Version mismatch:
            Server is strict -> close connection
            Server not strict -> wait for Initialization
    */

    /*
    Client receives info:
        Version ok: Initialization
        Version missmatch:
            Decide to close connection
            Or client sends initialize and skip packages if a parsing error occurs due to unknown options. See: Initialization
    */

    InfoData info(m_rcpVersion, m_handshakeVersion);

    if (!m_applicationId.empty())
    {
        info.setApplicationId(m_applicationId);
    }

    if (!m_applicationVersion.empty())
    {
        info.setApplicationVersion(m_applicationVersion);
    }

    // write info
    StringStreamWriter writer;
    writer.write(static_cast<char>(PACKET_TYPES_INFO));
    info.write(writer, false);

    setState(Handshake);
    sendData(writer);
}

void ParameterClient::disconnected()
{
    setState(Disconnected);

    // TODO
    // call callback?
}

void ParameterClient::received(std::istream & is)
{
    PacketParser::parse(is,
                        m_manager,
                        std::bind(&ParameterClient::_init, this, std::placeholders::_1),
                        std::bind(&ParameterClient::_version, this, std::placeholders::_1),
                        std::bind(&ParameterClient::_update, this, std::placeholders::_1, std::placeholders::_2),
                        std::bind(&ParameterClient::_remove, this, std::placeholders::_1));
}

void ParameterClient::_remove(const std::vector<int32_t>& id_data)
{
    if (m_state == FullyInitialized)
    {

#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_manager->m_mutex);
#endif

        for (auto i : id_data)
        {
            std::cout << "remove param: " << i << "\n";

            if (i == 0)
            {
                continue;
            }

            auto it = m_manager->params.find(i);
            if (it == m_manager->params.end())
            {
                continue;
            }

            // call parameter removed callbacks
            for (auto listener : m_listener)
            {
                listener->parameterRemoved(it->second);
            }

            // just remove
            m_manager->_removeParameterDirect(it->second);
        }
    }
    else
    {
        std::cerr << "received remove in state: " << to_string(m_state) << std::endl;
    }
}

void ParameterClient::_update(const std::vector<IParameterPtr> &parameter,
                              bool updateValue)
{
    if (m_state == FullyInitialized ||
        (!updateValue && m_state == Initialize))
    {
        for (auto& param : parameter)
        {
            rcp::ParameterClient::_update(std::dynamic_pointer_cast<rcp::IParameter>(param));

            if (m_state != FullyInitialized
                && m_manager->params.size() == m_expectedParameterCount)
            {
                setState(FullyInitialized);

                for (auto listener : m_listener)
                {
                    listener->initializeDone();
                }
            }
        }
    }
    else if (updateValue && m_state == Initialize)
    {
        // ignore update value packets?
    }
    else
    {
        // NOTE: don't close
        std::cerr << "received update in state: " << to_string(m_state) << std::endl;
    }
}

void ParameterClient::_init(const std::vector<int32_t>& idData)
{
    if (m_state == Initialize)
    {
        // as a client we expect only one rcp int

        if (!idData.empty())
        {
            auto i = idData.front();

            m_expectedParameterCount = i;

            std::cout << "expecting " << std::to_string(m_expectedParameterCount) << " parameters\n";

            // NOTE: handle special case
            if (m_expectedParameterCount == 0)
            {
                setState(FullyInitialized);

                for (auto listener : m_listener)
                {
                    listener->initializeDone();
                }
            }
        }
        else
        {
            // error?
            // warn("packet without data");
        }
    }
    else
    {
        errorAndClose("received initialize in state: " + to_string(m_state));
    }
}

void ParameterClient::_sendInitialize()
{
    char data[2] = {PACKET_TYPES_INITIALIZE,
                    char(0x80)}; // RcpInt(0)
    m_transporter.send(data, 2);
}

void ParameterClient::errorAndClose(const std::string &error)
{
    std::cerr << error << std::endl;
    disconnect();
}

void ParameterClient::setState(ClientState state)
{
    std::cout << "[ParameterClient][1.0] set state: " << m_state << " -> " << state << std::endl;
    m_state = state;
}

void ParameterClient::_version(const InfoData& info)
{
    if (m_state == Handshake)
    {
        std::cout << "server: " << info << std::endl;

        m_serverApplicationId = info.applicationId();
        m_serverApplicationVersion = info.applicationVersion();

        auto server_version_str = RcpVersion::to_string(info.protocolVersion()) + " - " + RcpVersion::to_string(info.handshakeVersion());

        for (auto& l : m_listener)
        {
            l->serverInfoReceived(server_version_str, m_serverApplicationId, m_serverApplicationVersion);
        }

        /*
            Compatibility between the server and a client is ensured
            if the server handshake-version is between the clients rcp-version
            and clients handshake-version (inclusive). See Protocol Flow for the
            version handshake and more details.
            */
        if (info.handshakeVersion() >= m_handshakeVersion &&
            info.handshakeVersion() <= m_rcpVersion)
        {
            setState(Initialize);

            // send init
            _sendInitialize();
        }
        else
        {
            errorAndClose("version mismatch!");
        }
    }
    else
    {
        errorAndClose("received info in state: " + to_string(m_state));
    }
}


}
}


