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

#ifndef RCP_WRITEABLE_H
#define RCP_WRITEABLE_H

#include <memory>

#include "Writer.h"

namespace rcp {

class Writeable
{
public:
    virtual ~Writeable();
    virtual void write(Writer& out, bool all) = 0;
};

Writer& operator<<(Writer& out, Writeable& Writeable);

using WriteablePtr = std::shared_ptr<Writeable>;

}

#endif // RCP_WRITEABLE_H
