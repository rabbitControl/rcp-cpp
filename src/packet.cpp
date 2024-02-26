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

    Packet::~Packet() {}

    std::ostream& operator<<(std::ostream& out, Packet& packet) {
        StreamWriter w(out);
        packet.write(w, false);
        return out;
    }
}
