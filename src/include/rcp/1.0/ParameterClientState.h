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

#ifndef RCP_V10_PARAMETERCLIENTSTATE_H
#define RCP_V10_PARAMETERCLIENTSTATE_H

#include <string>

namespace rcp {
namespace v10 {

enum ClientState {
    Unknown,
    Disconnected,
    Connected,
    Error,
    Handshake,
    Initialize,
    FullyInitialized
};

std::string to_string(ClientState state);
std::ostream& operator<<(std::ostream& out, ClientState state);

}
}

#endif // PARAMETERCLIENTSTATE_H
