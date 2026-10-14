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

#ifndef RCP_V010_PACKETPARSER_H
#define RCP_V010_PACKETPARSER_H

#include <istream>
#include <optional>

#include "IdData.h"
#include "InfoData.h"
#include "ParameterParser.h"
#include "types.h"

#include "../StreamTools.h"
#include "../ParameterManager.h"


namespace rcp {
namespace v010 {

class PacketParser
{
public:
    static void parse(std::istream& is,
                      ParameterManagerPtr manager,
                      const std::function<void(const IdDataPtr&)>& initCb,
                      const std::function<void(const InfoDataPtr&)>& versionCb,
                      const std::function<void(const IParameterPtr&)>& updateCb,
                      const std::function<void(const IdDataPtr&)>& removeCb,
                      const std::function<void()>& v10Callback)
    {
        // read command
        int result = is.get();

        if (result == std::char_traits<char>::eof())
        {
            throw std::runtime_error("PacketParser EOF");
        }

        // special case!
        // v010 packet
        // TODO: check for specific v10 info packet?
        if (result > COMMAND_UPDATEVALUE)
        {
            // packet type v10
            is.putback(static_cast<char>(result));
            v10Callback();
            return;
        }

        command_t command = static_cast<command_t>(result);

        if (command == TERMINATOR)
        {
            // return empty option
            return;
        }

        // check for valid command
        if (command <= COMMAND_INVALID ||
            command >= COMMAND_MAX_)
        {
            // return empty option
            return;
        }

        // create valid packet
        // Option<Packet> packet_option = Packet(command);

        //------------------------------------
        // handle update value
        if (command == COMMAND_UPDATEVALUE)
        {
            auto param = ParameterParser::parseUpdateValue(is);
            updateCb(param);
            return;
        }


        std::optional<uint64_t> timestamp;
        bool got_data = false;

        //------------------------------------
        // read options
        char ch;
        while (is.get(ch))
        {
            // read option prefix
            packet_options_t packet_prefix = static_cast<packet_options_t>(ch);

            if (packet_prefix == TERMINATOR)
            {
                break;
            }

            if (packet_prefix < PACKET_OPTIONS_TIMESTAMP
                || packet_prefix > PACKET_OPTIONS_DATA)
            {
                std::cerr << "error: [0.1.0] unknown packet option: 0x" << std::hex << packet_prefix << "\n";
                std::flush(std::cerr);
                break;
            }

            switch (packet_prefix)
            {
            case PACKET_OPTIONS_TIMESTAMP:
            {
                uint64_t v = readFromStream(is, v);

                CHECK_STREAM;

                timestamp = v;
                std::cout << "packet timestamp: " << std::to_string(timestamp.value()) << std::endl;
                break;
            }
            // packet data
            case PACKET_OPTIONS_DATA:

                got_data = true;

                switch (command)
                {
                case COMMAND_INVALID:
                case COMMAND_MAX_:
                    // ERROR
                    break;

                case COMMAND_INFO:
                    versionCb(InfoData::parse(is));
                    break;

                case COMMAND_INITIALIZE:
                    initCb(IdData::parse(is));
                    break;

                case COMMAND_DISCOVER:
                    // TODO
                    // exect ID-data or null
                    // parameter-id followed by terminator
                    break;

                case COMMAND_UPDATE:
                    updateCb(ParameterParser::parse(is, manager));
                    break;

                case COMMAND_REMOVE:
                    removeCb(IdData::parse(is));
                    break;

                case COMMAND_UPDATEVALUE:
                    // handled above
                    break;
                } // switch

                break;
            } // switch
        } // while

        // deal with "empty" packets
        if (!got_data)
        {
            switch (command)
            {
            case COMMAND_INFO:
                versionCb(nullptr);
                break;

            case COMMAND_INITIALIZE:
                initCb(nullptr);
                break;

            default:
                break;
            }
        }
    }
};

}
}

#endif // PACKETPARSER_H
