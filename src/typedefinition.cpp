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

#include "typedefinition.h"
#include "streamwriter.h"

#include "type_enum.h"
#include "type_string.h"
#include "type_uri.h"

namespace rcp {

template<typename T,
         datatype_t type_id,
         td_types t
         >
std::ostream& operator<<(std::ostream& out, rcp::TypeDefinition<T, type_id, t>& type)
{
    StreamWriter w(out);
    type.write(w);
    return out;
}


}
