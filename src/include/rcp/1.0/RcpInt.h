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

#ifndef RCP_V10_RCPINT_H
#define RCP_V10_RCPINT_H

#include <iostream>
#include <istream>

#include "../Writeable.h"

namespace rcp {
namespace v10 {

class RcpInt
    : public rcp::Writeable
{
public:
    static const uint32_t MAX_VALUE = 268435455; // 2^28 - 1

    static RcpInt parse(std::istream& is)
    {
        uint32_t v = 0;

        uint8_t i = 0;
        while (i < 4)
        {
            int result = is.get();
            if (result == std::char_traits<char>::eof())
            {
                throw std::runtime_error("RcpInt EOF");
            }

            uint8_t d = static_cast<uint8_t>(result);

            // "big endian"
            v = v << 7;
            v |= (d & 0x7F);

            // "little endian" - not used
            // v |= ((d & 0x7F) << (i*7));

            if (d & 0x80)
            {
                break;
            }

            i++;
        }

        if (i >= 4)
        {
            throw std::runtime_error("RcpInt not terminated");
        }

        return RcpInt(v);
    }

public:
    RcpInt(uint32_t id)
        : m_value(id)
    {
    }

    uint32_t value() const
    {
        return m_value;
    }

public:
    // Writeable interface
    void write(Writer &out, bool all)
    {
        // size check value to be <= (2^28 - 1)!
        if (m_value > RcpInt::MAX_VALUE)
        {
            throw std::runtime_error("invalid rcp int value");
        }

        // big endian

        // NOTE: This implementation appears to be more efficient
        // than the implementation below.
        // NOTE: while this method can also be used to write little
        // endian RcpInt, using the big-endian version leads to this
        // implementation.
        if (m_value < 0x4000)
        {
            if (m_value < 0x80)
            {
                // 1 byte
                out.write(static_cast<char>((m_value & 0x7F) | 0x80));
            }
            else
            {
                // 2 bytes
                out.write(static_cast<char>((m_value >> 7) & 0x7F));
                out.write(static_cast<char>((m_value & 0x7F) | 0x80));
            }
        }
        else
        {
            if (m_value < 0x200000)
            {
                //3 bytes
                out.write(static_cast<char>((m_value >> 14) & 0x7F));
                out.write(static_cast<char>((m_value >> 7) & 0x7F));
                out.write(static_cast<char>((m_value & 0x7F) | 0x80));
            }
            else
            {
                // 4 bytes
                out.write(static_cast<char>((m_value >> 21) & 0x7F));
                out.write(static_cast<char>((m_value >> 14) & 0x7F));
                out.write(static_cast<char>((m_value >> 7) & 0x7F));
                out.write(static_cast<char>((m_value & 0x7F) | 0x80));
            }
        }



        // little endian - not used
        // uint8_t i = 0;
        // uint32_t v = m_value;
        // while (i < 4)
        // {
        //     uint8_t d = v & 0x7F;

        //     v = v >> 7;

        //     if (v == 0)
        //     {
        //         out.write(char(d | 0x80));
        //         break;
        //     }
        //     else
        //     {
        //         out.write(d);
        //     }

        //     i++;
        // }
    }

private:
    uint32_t m_value;

};


}
}


#endif // RCPINT_H
