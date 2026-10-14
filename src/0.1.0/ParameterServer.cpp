#include "rcp/0.1.0/ParameterServer.h"

#include "rcp/StringStreamWriter.h"

#include "rcp/0.1.0/InfoData.h"
#include "rcp/0.1.0/Version.h"
#include "rcp/0.1.0/IdData.h"
#include "rcp/0.1.0/PacketWriter.h"
#include "rcp/0.1.0/PacketParser.h"
#include "rcp/0.1.0/ParameterParser.h"

namespace rcp {
namespace v010 {

ParameterServer::ParameterServer()
    : rcp::ParameterServer(std::make_shared<GroupParameter>(0, "root"))
{
}


ParameterServer::ParameterServer(ServerTransporter& transporter)
    : rcp::ParameterServer(transporter, std::make_shared<GroupParameter>(0, "root"))
{
}

BooleanParameterPtr ParameterServer::createBooleanParameter(const std::string &label, const IGroupParameterPtr &group)
{
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
    if (id != 0)
    {
        auto p = StringParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

RGBAParameterPtr ParameterServer::createRGBAParameter(const std::string &label, const IGroupParameterPtr &group)
{
    int16_t id = _getNextId();
    if (id != 0)
    {
        auto p = RGBAParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

BangParameterPtr ParameterServer::createBangParameter(const std::string &label, const IGroupParameterPtr &group)
{
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
    if (id != 0)
    {
        auto p = RGBParameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}

URIParameterPtr ParameterServer::createURIParameter(const std::string &label, const IGroupParameterPtr &group)
{
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
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
    int16_t id = _getNextId();
    if (id != 0)
    {
        auto p = IPv6Parameter::create(id);
        m_manager->_addParameterDirect(label, p, group);

        return p;
    }

    throw std::runtime_error("no valid id...");
}


void ParameterServer::_sendParameterFull(const rcp::IParameterPtr& parameter, ServerTransporter& transporter, void *id)
{
    StringStreamWriter writer;

    PacketWriter::write(COMMAND_UPDATE, parameter, writer, true);

    transporter.sendToOne(writer.getBuffer(), id);

    if (parameter->isGroup())
    {
        auto group_param = std::dynamic_pointer_cast<GroupParameter>(parameter);
        if (group_param)
        {
            for (auto& child : group_param->children())
            {
                _sendParameterFull(child.second, transporter, id);
            }
        }
    }
}


void ParameterServer::_init(const IdDataPtr& idData, ServerTransporter & transporter, void *id)
{
    if (idData)
    {
        //
    }

    for (auto& child : m_manager->getRootGroup()->children())
    {
        _sendParameterFull(child.second, transporter, id);
    }

    StringStreamWriter writer;

    // send initialize to mark end of init
    writer.write(static_cast<char>(COMMAND_INITIALIZE));
    writer.write(Terminator);

    transporter.sendToOne(writer.getBuffer(), id);
}

void ParameterServer::_version(const InfoDataPtr& info, ServerTransporter &transporter, void *id)
{
    if (info)
    {
        // log info data
        for (auto listener : m_listener)
        {
            listener->clientInfoReceived(info->getApplicationId(), info->getVersion());
        }
    }
    else
    {
        // no data, respond with version
        auto version = InfoData::create(RCP_SPECIFICATION_VERSION, m_applicationId);

        StringStreamWriter writer;

        PacketWriter::write(COMMAND_INFO, version, writer, false);

        transporter.sendToOne(writer.getBuffer(), id);

        // ask for version
        StringStreamWriter writer1;
        writer1.write(static_cast<char>(COMMAND_INFO));
        writer.write(Terminator);
        transporter.sendToOne(writer1.getBuffer(), id);
    }
}


bool ParameterServer::update()
{
    if (transporterList.empty())
    {
        return false;
    }

    // protect lists to be used from multiple threads
    m_manager->lock();

    // send removes
    for (auto& p : m_manager->removedParameter)
    {
        StringStreamWriter writer;

        PacketWriter::write(COMMAND_REMOVE, p.second, writer, false);

        sendData(writer);
    }
    m_manager->removedParameter.clear();

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

    // unlock mutex
    m_manager->unlock();

    return false;
}



void ParameterServer::received(std::istream &is, ServerTransporter &transporter, void *id)
{
    PacketParser::parse(is,
                        m_manager,
                        std::bind(&ParameterServer::_init, this, std::placeholders::_1, std::ref(transporter), id),
                        std::bind(&ParameterServer::_version, this, std::placeholders::_1, std::ref(transporter), id),
                        std::bind(&ParameterServer::_update, this, std::placeholders::_1),
                        [](const rcp::v010::IdDataPtr& id)
    {
        // remove: nop
    },
                        [this, &transporter, id]
    {
        // v10 callback!
        // TODO: log error
        transporter.closeClient(id);
    });
}


}
}








