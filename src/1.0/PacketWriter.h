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

#ifndef RCP_V10_PACKETWRITER_H
#define RCP_V10_PACKETWRITER_H

#include "../IParameter.h"
#include "../Writeable.h"

#include "InfoData.h"
#include "RcpInt.h"
#include "types.h"

namespace rcp {
namespace v10 {

class PacketWriter
{
public:
    static void writeUpdate(rcp::IParameterPtr parameter, Writer& writer, bool all)
    {
        if (!all && ((parameter->getDatatype() == DATATYPE_BANG && parameter->m_changed.empty()) ||
                     parameter->onlyValueChanged()))
        {
            writer.write(static_cast<char>(PACKET_TYPES_UPDATEVALUE));
        }
        else
        {
            writer.write(static_cast<char>(PACKET_TYPES_UPDATE));
        }

        ParameterWriter::write(std::dynamic_pointer_cast<IParameter>(parameter), writer, all);
    }

    static void writeRemove(rcp::IParameterPtr parameter, Writer& writer)
    {
        writer.write(static_cast<char>(PACKET_TYPES_REMOVE));
        RcpInt(parameter->getId()).write(writer, true);
    }

    static void writeInit(uint32_t count, Writer& writer)
    {
        writer.write(static_cast<char>(PACKET_TYPES_INITIALIZE));
        RcpInt(count).write(writer, true);
    }

    static void writeInfo(InfoData& info, Writer& writer)
    {
        writer.write(static_cast<char>(PACKET_TYPES_INFO));
        info.write(writer, true);
    }
};

}
}

#endif // RCP_V10_PACKETWRITER_H
