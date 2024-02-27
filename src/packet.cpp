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

#include "packet.h"

#include "streamwriter.h"

namespace rcp {


Option<Packet>
Packet::parse(std::istream& is, std::shared_ptr<IParameterManager> manager)
{
    // read command
    command_t command = static_cast<command_t>(is.get());

    if (command == TERMINATOR) {
        // return empty option
        return Option<Packet>();
    }

    // check for valid command
    if (is.eof() ||
        command <= COMMAND_INVALID ||
        command >= COMMAND_MAX_)
    {
        // return empty option
        return Option<Packet>();
    }

    // create valid packet
    Option<Packet> packet_option = Packet(command);

    //------------------------------------
    // handle update value
    if (packet_option.value().getCommand() == COMMAND_UPDATEVALUE)
    {
        ParameterPtr param = ParameterParser::parseUpdateValue(is);

        if (param != nullptr) {
            packet_option.value().setData(param);
            return packet_option;
        }
        else
        {
            // empty packet... means fail...?
            return Option<Packet>();
        }
    }


    //------------------------------------
    // read options
    while (!is.eof())
    {
        // read option prefix
        packet_options_t packet_prefix = static_cast<packet_options_t>(is.get());

        if (packet_prefix == TERMINATOR)
        {
            return packet_option;
        }

        if (is.eof()
            || is.fail())
        {
            return Option<Packet>();
        }

        if (packet_prefix < PACKET_OPTIONS_TIMESTAMP
            || packet_prefix > PACKET_OPTIONS_DATA)
        {
            std::cout << "error: unknown packet option: " << packet_prefix << "\n";
            break;
        }

        switch (packet_prefix)
        {
        case PACKET_OPTIONS_TIMESTAMP:
        {
            uint64_t v = readFromStream(is, v);
            CHECK_STREAM

                    packet_option.value().setTimestamp(v);
            break;
        }
        // packet data
        case PACKET_OPTIONS_DATA:

            // already have data?
            if (packet_option.value().hasData())
            {
                // error - data already set
                // throw exception here?
                return Option<Packet>();
            }

            switch (command)
            {
            case COMMAND_INVALID:
            case COMMAND_MAX_:
                // ERROR
                break;

            case COMMAND_INFO:
            {
                // get infodata
                InfoDataPtr info_data = InfoData::parse(is);
                if (info_data != nullptr)
                {
                    packet_option.value().setData(info_data);
                }
                else
                {
                    return Option<Packet>();
                }

                break;
            }

            case COMMAND_INITIALIZE:
            {
                // exect ID-data or null
                IdDataPtr id_data = IdData::parse(is);

                if (id_data != nullptr)
                {
                    packet_option.value().setData(id_data);
                }

                break;
            }

            case COMMAND_DISCOVER:
                // TODO
                // exect ID-data or null
                // parameter-id followed by terminator
                break;

            case COMMAND_UPDATE:
            {
                // we expect a Parameter
                ParameterPtr param = ParameterParser::parse(is, manager);

                if (param != nullptr)
                {
                    packet_option.value().setData(param);
                }

                break;
            }

            case COMMAND_REMOVE:
            {
                // we expect ID Data
                IdDataPtr id_data = IdData::parse(is);

                if (id_data != nullptr)
                {
                    packet_option.value().setData(id_data);
                }

                break;
            }

            case COMMAND_UPDATEVALUE:
                // handled above
                break;
            } // switch

            break;
        } // switch
    } // while

    return packet_option;
}



std::ostream& operator<<(std::ostream& out, Packet& packet) {
    StreamWriter w(out);
    packet.write(w, false);
    return out;
}


}
