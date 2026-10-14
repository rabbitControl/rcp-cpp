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

#ifndef RCP_V10_RCPSTRING_H
#define RCP_V10_RCPSTRING_H

#include <string>
#include <vector>

#include "../Writeable.h"

#include "RcpInt.h"

namespace rcp {
namespace v10 {

class RcpString
    : public std::string
    , public rcp::Writeable
{
public:
    static RcpString parse(std::istream& is)
    {
        auto size = RcpInt::parse(is).value();

        // read to buffer
        std::vector<char> buffer(size);
        is.read(buffer.data(), size);

        return RcpString{std::string(buffer.data(), size)};
    }

public:
    RcpString(const std::string& str)
        : std::string(str)
    {}

public:
    // Writeable interface
    void write(Writer &out, bool all)
    {
        RcpInt(length()).write(out, true);
        out.write(*this, false);
    }
};

}
}

#endif // RCPSTRING_H
