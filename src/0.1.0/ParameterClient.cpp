#include "rcp/0.1.0/IdData.h"
#include "rcp/0.1.0/InfoData.h"
#include "rcp/0.1.0/ParameterClient.h"
#include "rcp/0.1.0/GroupParameter.h"
#include "rcp/0.1.0/PacketWriter.h"
#include "rcp/0.1.0/PacketParser.h"
#include "rcp/0.1.0/types.h"
#include "rcp/0.1.0/Version.h"


namespace rcp {
namespace v010 {


ParameterClient::ParameterClient(ClientTransporter& transporter,
                                 const std::function<void()>& v10Callback)
    : rcp::ParameterClient(transporter)
    , m_v10Callback(v10Callback)
{
    m_manager = ParameterManager::create(std::make_shared<GroupParameter>(0, "root"));
}

std::string ParameterClient::getServerVersion() const
{
    return m_serverVersion;
}

std::string ParameterClient::getServerApplicationId() const
{
    return m_serverApplicationId;
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

        if (p.second->onlyValueChanged())
        {
            PacketWriter::write(COMMAND_UPDATEVALUE, p.second, writer, false);
        }
        else
        {
            PacketWriter::write(COMMAND_UPDATE, p.second, writer, false);
        }

        sendData(writer);
    }
    m_manager->dirtyParameter.clear();

    m_manager->unlock();
}


void ParameterClient::received(std::istream & is)
{
    PacketParser::parse(is,
                        m_manager,
                        std::bind(&ParameterClient::_init, this, std::placeholders::_1),
                        std::bind(&ParameterClient::_version, this, std::placeholders::_1),
                        std::bind(&ParameterClient::_update, this, std::placeholders::_1),
                        std::bind(&ParameterClient::_remove, this, std::placeholders::_1),
                        m_v10Callback);

    // TODO: log v10 callback?
}

void ParameterClient::_remove(const IdDataPtr &id_data)
{
    if (id_data)
    {
        // std::cout << "remove param: " << id_data->getId() << "\n";

        auto cached_param = m_manager->getParameter(id_data->getId());
        if (cached_param)
        {
            // parameter is in list, remove it
            // std::cout << "removing exisiting parameter: " << id_data->getId() << "\n";

            // call parameter removed callbacks
            for (auto listener : m_listener)
            {
                listener->parameterRemoved(cached_param);
            }

            // remove it (direct)
            m_manager->_removeParameterDirect(cached_param);
        }
        else
        {
            // std::cout << "parameter not in list!: " << id_data->getId() << "\n";
        }
    }
    else
    {
        // std::cerr << "data not a parameter\n";
    }
}

void ParameterClient::_init(const IdDataPtr &idData)
{
    for (auto listener : m_listener)
    {
        listener->initializeDone();
    }
}

void ParameterClient::_sendInitialize()
{
    char data[2];
    data[0] = 0x02;
    data[1] = 0x00;
    m_transporter.send(data, 2);
}

void ParameterClient::_version(const InfoDataPtr &info)
{
    if (info)
    {
        m_serverApplicationId = info->getApplicationId();
        m_serverVersion = info->getVersion();

        std::cout << "from server: " << m_serverVersion << " - " << m_serverApplicationId << "\n";

        for (auto listener : m_listener)
        {
            listener->serverInfoReceived(m_serverVersion, m_serverApplicationId);
        }

        // TODO: compare RCP version
        // only send initialize after compatibility-check
        bool version_ok = true;

        if (!version_ok)
        {
            // TODO: disconnect from server
        }

        if (!m_initializeSent &&
            version_ok)
        {
            _sendInitialize();

            // only send initialize once
            m_initializeSent = true;
        }
    }
    else
    {
        // no data, respond with version
        WriteablePtr version = InfoData::create(RCP_SPECIFICATION_VERSION, m_applicationId);

        StringStreamWriter writer;

        PacketWriter::write(COMMAND_INFO, version, writer, false);

        sendData(writer);
    }
}


}
}


