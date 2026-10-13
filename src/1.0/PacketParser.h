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

#ifndef RCP_V10_PACKETPARSER_H
#define RCP_V10_PACKETPARSER_H

#include <istream>
#include <optional>

#include "../StreamTools.h"
#include "../ParameterManager.h"

#include "InfoData.h"
#include "RcpInt.h"
#include "types.h"

namespace rcp {
namespace v10 {

class PacketParser
{
public:
    static void parse(std::istream& is,
                      ParameterManagerPtr manager,
                      const std::function<void(const std::vector<int32_t>&)>& initCb,
                      const std::function<void(const InfoData&)>& versionCb,
                      const std::function<void(const std::vector<IParameterPtr>&, bool updateValue)>& updateCb,
                      const std::function<void(const std::vector<int32_t>&)>& removeCb,
                      const std::function<void()>& versionV010Cb = []{})
    {

        int packet_prefix = is.get();

        if (packet_prefix == std::char_traits<char>::eof())
        {
            throw std::runtime_error("PacketParser EOF");
        }

        // special case!
        // v010 info packet
        if (packet_prefix == 1)
        {
            //
            is.putback(static_cast<char>(packet_prefix));
            versionV010Cb();
            return;
        }

        // read command
        packet_types_t packet_type = static_cast<packet_types_t>(packet_prefix & 0x1F);

        // check for valid command
        if (packet_type <= PACKET_TYPES_INVALID ||
            packet_type > PACKET_TYPES_REMOVE)
        {
            return;
        }


        bool ts = (packet_prefix & 0x80) > 0;

        bool rsv1 = (packet_prefix & 0x40) > 0;
        bool rsv2 = (packet_prefix & 0x20) > 0;


        std::optional<uint64_t> timestamp;

        if (ts)
        {
            timestamp = readFromStream(is, uint64_t{});
        }


        switch(packet_type)
        {
        case PACKET_TYPES_INFO:
            versionCb(InfoData::parse(is));
            break;

        case PACKET_TYPES_INITIALIZE:
        {
            // TODO: is this correct?
            std::vector<int32_t> ids;
            while(is.good())
            {
                ids.push_back(RcpInt::parse(is).value());

                if (is.peek() == std::char_traits<char>::eof())
                {
                    break;
                }
            }
            initCb(ids);
            break;
        }
        case PACKET_TYPES_REMOVE:
        {
            std::vector<int32_t> ids;
            while(is.good())
            {
                ids.push_back(RcpInt::parse(is).value());

                if (is.peek() == std::char_traits<char>::eof())
                {
                    break;
                }
            }
            removeCb(ids);
            break;
        }
        case PACKET_TYPES_UPDATE:
        {
            std::vector<IParameterPtr> params;
            while(is.good())
            {
                auto param = ParameterParser::parse(is, manager);
                if (param)
                {
                    params.push_back(param);
                }

                if (is.peek() == std::char_traits<char>::eof())
                {
                    break;
                }
            }
            updateCb(params, false);
            break;
        }
        case PACKET_TYPES_UPDATEVALUE:
        {
            std::vector<IParameterPtr> params;
            while(is.good())
            {
                auto param = ParameterParser::parseUpdateValue(is);
                if (param)
                {
                    params.push_back(param);
                }

                if (is.peek() == std::char_traits<char>::eof())
                {
                    break;
                }
            }
            updateCb(params, true);
            break;
        }

        default:
            // nop
            break;
        }
    }
};

}
}

#endif // PACKETPARSER_H
