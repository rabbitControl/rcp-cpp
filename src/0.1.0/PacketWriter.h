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

#ifndef RCP_V010_PACKERWRITER_H
#define RCP_V010_PACKERWRITER_H

#include "../IParameter.h"
#include "../Writeable.h"

namespace rcp {
namespace v010 {

class PacketWriter
{
public:
    static void write(command_t command, rcp::IParameterPtr parameter, Writer& writer, bool all)
    {
        if (command == COMMAND_REMOVE)
        {
            writer.write(static_cast<char>(command));
            writer.write(static_cast<char>(PACKET_OPTIONS_DATA));

            writer.write(int16_t(parameter->getId()));

            writer.write(Terminator);
        }
        else if (command == COMMAND_UPDATEVALUE)
        {
            writer.write(static_cast<char>(command));
            ParameterWriter::write(parameter, writer, all);
        }
        else if (command == COMMAND_UPDATE)
        {
            writer.write(static_cast<char>(command));
            writer.write(static_cast<char>(PACKET_OPTIONS_DATA));

            ParameterWriter::write(parameter, writer, all);

            writer.write(Terminator);
        }
    }

    static void write(command_t command, rcp::WriteablePtr writable, Writer& writer, bool all)
    {
        writer.write(static_cast<char>(command));
        writer.write(static_cast<char>(PACKET_OPTIONS_DATA));

        writable->write(writer, all);

        writer.write(Terminator);
    }
};

}
}

#endif // PACKERWRITER_H
