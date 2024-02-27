/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2024
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCP_PACKET_H
#define RCP_PACKET_H

#include "parameter_intern.h"
#include "parameter_range.h"
#include "parameter_custom.h"
#include "parameter_parser.h"
#include "infodata.h"
#include "iddata.h"

namespace rcp {

class Packet
    : public Writeable
{

public:
    static Option<Packet> parse(std::istream& is, std::shared_ptr<IParameterManager> manager = nullptr);

public:
    Packet()
        : m_command(COMMAND_INVALID)
    {}

    Packet(enum command_t cmd)
        : m_command(cmd)
        , m_timestamp(0)
    {
        m_timestamp.clearValue();
    }

    Packet(enum command_t cmd, ParameterPtr& data)
        : m_command(cmd)
        , m_timestamp(0)
    {
        m_timestamp.clearValue();
        setData(data);
    }

    Packet(enum command_t cmd, WriteablePtr& data)
        : m_command(cmd)
        , m_timestamp(0)
    {
        m_timestamp.clearValue();
        setData(data);
    }

    Packet(enum command_t cmd, uint64_t timestamp, ParameterPtr& data) :
        m_command(cmd)
        , m_timestamp(timestamp)
    {
        setData(data);
    }

    Packet(const Packet& other)
    {
        m_command = other.getCommand();

        if (other.m_timestamp.hasValue())
        {
            m_timestamp = other.m_timestamp;
        }

        if (other.m_data.hasValue())
        {
            m_data = other.m_data;
        }
    }

    Packet& operator=(const Packet& other)
    {
        m_command = other.getCommand();

        if (other.m_timestamp.hasValue())
        {
            m_timestamp = other.m_timestamp;
        }
        else
        {
            m_timestamp.clearValue();
        }

        if (other.m_data.hasValue())
        {
            m_data = other.m_data;
        }
        else
        {
            m_data.clearValue();
        }

        return *this;
    }


    // writeable interface
    virtual void write(Writer& out, bool all)
    {
        out.write(static_cast<char>(m_command));

        if (m_command == COMMAND_UPDATEVALUE)
        {
            // only parameter
            ParameterPtr param = std::dynamic_pointer_cast<IParameter>(m_data.value());
            if (param)
            {
                param->writeUpdateValue(out);
            }
        }
        else
        {
            if (m_timestamp.hasValue())
            {
                out.write(static_cast<char>(PACKET_OPTIONS_TIMESTAMP));
                out.write(m_timestamp.value());
            }

            if (m_data.hasValue())
            {
                out.write(static_cast<char>(PACKET_OPTIONS_DATA));
                m_data.value()->write(out, all);
            }

            // terminator
            out.write(static_cast<char>(TERMINATOR));
        }
    }


    // public methods

    //----------------------
    // command
    void setCommand(enum command_t cmd)
    {
        m_command = cmd;
    }
    enum command_t getCommand() const
    {
        return m_command;
    }

    //----------------------
    // option - timestamp
    bool hasTimestamp() const
    {
        return m_timestamp.hasValue();
    }
    void setTimestamp(uint64_t timestamp)
    {
        m_timestamp = timestamp;
    }
    uint64_t getTimestamp() const
    {
        if (m_timestamp.hasValue())
        {
            return m_timestamp.value();
        }

        return 0;
    }
    void clearTimestamp()
    {
        m_timestamp.clearValue();
    }

    //----------------------
    // option - data
    bool hasData() const
    {
        return m_data.hasValue();
    }
    void setData(const WriteablePtr& data)
    {
        m_data = data;
    }
    WriteablePtr getData() const
    {
        if (m_data.hasValue())
        {
            return m_data.value();
        }

        return nullptr;
    }
    void clearData()
    {
        m_data.clearValue();
    }


private:

    // mandatory
    enum command_t m_command;

    // options
    Option<uint64_t> m_timestamp;
    Option<WriteablePtr> m_data;
};


std::ostream& operator<<(std::ostream& out, Packet& Packet);
}

#endif
