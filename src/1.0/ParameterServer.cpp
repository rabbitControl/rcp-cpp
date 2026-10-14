#include "ParameterServer.h"

#include "../StringStreamWriter.h"

#include "PacketParser.h"
#include "PacketWriter.h"

namespace rcp {
namespace v10 {

ParameterServer::ParameterServer()
    : rcp::ParameterServer(std::make_shared<GroupParameter>(0, "root"))
    , m_rcpVersion{0, 9}
    , m_handshakeVersion{0, 9}
{
}


ParameterServer::ParameterServer(ServerTransporter& transporter)
    : rcp::ParameterServer(transporter, std::make_shared<GroupParameter>(0, "root"))
    , m_rcpVersion{0, 9}
    , m_handshakeVersion{0, 9}
{    
}

void ParameterServer::setApplicationVersion(const std::string &version)
{
    m_applicationVersion = version;
}

std::string ParameterServer::applicationVersion() const
{
    return m_applicationVersion;
}

RcpVersion ParameterServer::getRcpVersion() const
{
    return m_rcpVersion;
}

void ParameterServer::setHandshakeVersion(const RcpVersion &version)
{
    if (version > m_rcpVersion)
    {
        // not allowed
        return;
    }

    m_handshakeVersion = version;
}

RcpVersion ParameterServer::getHandshakeVersion() const
{
    return m_handshakeVersion;
}

void ParameterServer::setIsStrict(bool strict)
{
    m_isStrict = strict;
}

bool ParameterServer::getIsStrict() const
{
    return m_isStrict;
}

BooleanParameterPtr ParameterServer::createBooleanParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = BooleanParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

Int8ParameterPtr ParameterServer::createInt8Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = Int8Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

UInt8ParameterPtr ParameterServer::createUInt8Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = UInt8Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

Int16ParameterPtr ParameterServer::createInt16Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = Int16Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

UInt16ParameterPtr ParameterServer::createUInt16Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = UInt16Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

Int32ParameterPtr ParameterServer::createInt32Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = Int32Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

UInt32ParameterPtr ParameterServer::createUInt32Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = UInt32Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

Int64ParameterPtr ParameterServer::createInt64Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = Int64Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

UInt64ParameterPtr ParameterServer::createUInt64Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = UInt64Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

Float32ParameterPtr ParameterServer::createFloat32Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = Float32Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

Float64ParameterPtr ParameterServer::createFloat64Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = Float64Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

StringParameterPtr ParameterServer::createStringParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = StringParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

BangParameterPtr ParameterServer::createBangParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = BangParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

GroupParameterPtr ParameterServer::createGroupParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = GroupParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

EnumParameterPtr ParameterServer::createEnumParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = EnumParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

RGBParameterPtr ParameterServer::createRGBParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = RGBParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

RGBAParameterPtr ParameterServer::createRGBAParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = RGBAParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

RGBFloatParameterPtr ParameterServer::createRGBFloatParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = RGBFloatParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

RGBAFloatParameterPtr ParameterServer::createRGBAFloatParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = RGBAFloatParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

URIParameterPtr ParameterServer::createURIParameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = URIParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

IPv4ParameterPtr ParameterServer::createIPv4Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = IPv4Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

IPv6ParameterPtr ParameterServer::createIPv6Parameter(const std::string &label, const IGroupParameterPtr &group)
{
    uint32_t id = _getNextId();
    if (id != 0)
    {
        auto p = IPv6Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}


void ParameterServer::_sendParameterFull(const IParameterPtr& parameter, ServerTransporter& transporter, void *id)
{
    StringStreamWriter writer;

    PacketWriter::writeUpdate(parameter, writer, true);

    transporter.sendToOne(writer.getBuffer(), id);

    if (parameter->isGroup())
    {
        auto group_param = std::dynamic_pointer_cast<GroupParameter>(parameter);
        if (group_param)
        {
            for (auto& child : group_param->children())
            {
                _sendParameterFull(std::dynamic_pointer_cast<IParameter>(child.second), transporter, id);
            }
        }
    }
}


void ParameterServer::_init(const std::vector<int32_t>& idData, ServerTransporter & transporter, void *id)
{
    setState(id, ClientState::Initialize);


    StringStreamWriter writer;

    // send initialize with amount of parameters
    PacketWriter::writeInit(m_manager->params.size(), writer);

    transporter.sendToOne(writer.getBuffer(), id);


    // TODO: use id data?
    // NOTE: match idData content with parameters in list

    // for (auto& child : getRootGroup()->children())
    // {
    //     _sendParameterFull(std::dynamic_pointer_cast<IParameter>(child.second), transporter, id);
    // }

    // TODO: send in batch?
    for (auto& param : m_manager->params)
    {
        StringStreamWriter writer;

        PacketWriter::writeUpdate(param.second, writer, true);

        transporter.sendToOne(writer.getBuffer(), id);
    }

    // if (idData.empty())
    // {
    // }
    // else
    // {
    //     for (auto i : idData)
    //     {
    //         auto it = params.find(i);
    //         if (it != params.end())
    //         {

    //         }
    //         else
    //         {
    //             //ERROR!
    //         }
    //     }
    // }



    setState(id, ClientState::FullyInitialized);
}

void ParameterServer::_version(const InfoData& infoData, ServerTransporter &transporter, void *id)
{
    std::cout << "client: " << infoData << std::endl;

    // check version
    /*
        Compatibility between the server and a client is ensured
        if the server handshake-version is between the clients rcp-version
        and clients handshake-version (inclusive). See Protocol Flow for the
        version handshake and more details.
    */
    if (m_handshakeVersion <= infoData.protocolVersion() &&
        m_handshakeVersion >= infoData.handshakeVersion())
    {
        std::cout << "VERSION OK - waiting for init\n";

        setState(id, ClientState::Handshake);

        // waiting for init
    }
    else if (m_isStrict)
    {
        // version not ok and strict
        std::cout << "VERSION NOT OK - disconnect\n";
        transporter.closeClient(id);
        return;
    }
    else
    {
        // version is not ok, but server knows that it will be all right
        // waiting for init

        setState(id, ClientState::Handshake);
    }


    _sendVersion(transporter, id);
}

void ParameterServer::_sendVersion(ServerTransporter &transporter, void *id)
{
    // send info to client
    InfoData info(m_rcpVersion, m_handshakeVersion);

    if (!m_applicationId.empty())
    {
        info.setApplicationId(m_applicationId);
    }

    if (!m_applicationVersion.empty())
    {
        info.setApplicationVersion(m_applicationVersion);
    }

    StringStreamWriter writer;

    PacketWriter::writeInfo(info, writer);

    transporter.sendToOne(writer.getBuffer(), id);
}


bool ParameterServer::update()
{
    if (transporterList.empty())
    {
        return false;
    }

    // protect lists to be used from multiple threads
    m_manager->lock();

    // TODO: send in batch?
    // send removes
    for (auto& p : m_manager->removedParameter)
    {
        StringStreamWriter writer;

        PacketWriter::writeRemove(p.second, writer);

        // TODO: allow to send to fully initialized clients!

        sendData(writer);
    }
    m_manager->removedParameter.clear();

    // send updates
    for (auto& p : m_manager->dirtyParameter)
    {
        StringStreamWriter writer;

        PacketWriter::writeUpdate(p.second, writer, false);

        // TODO: allow to send to fully initialized clients!

        sendData(writer);
    }
    m_manager->dirtyParameter.clear();

    // unlock mutex
    m_manager->unlock();

    return false;
}


void ParameterServer::_update(const std::vector<IParameterPtr>& params)
{
    for (auto& param : params)
    {
        rcp::ParameterServer::_update(param);
    }
}

void ParameterServer::setState(void *client, ClientState state)
{
    if (m_clientStates.find(client) != m_clientStates.end())
    {
        std::cout << "[ParameterServer][1.0] client state " << client << ": " << m_clientStates[client] << " -> " << state << std::endl;
    }
    else
    {
        std::cout << "[ParameterServer][1.0] client state " << client << ": " << state << std::endl;
    }

    m_clientStates[client] = state;
}


void ParameterServer::connected(void* client)
{
    setState(client, ClientState::Connected);
}

void ParameterServer::disconnected(void* client)
{
    m_clientStates.erase(client);
}


void ParameterServer::received(std::istream &is, ServerTransporter &transporter, void *id)
{
    PacketParser::parse(is,
                        m_manager,
                        std::bind(&ParameterServer::_init, this, std::placeholders::_1, std::ref(transporter), id),
                        std::bind(&ParameterServer::_version, this, std::placeholders::_1, std::ref(transporter), id),
                        std::bind(&ParameterServer::_update, this, std::placeholders::_1),
                        [](const std::vector<int32_t>& id)
    {
        // remove: nop
    },
                        [this, &transporter, id]
    {
        // v010 info packet!
        // just send v10 info back
        _sendVersion(transporter, id);
    });
}


}
}








