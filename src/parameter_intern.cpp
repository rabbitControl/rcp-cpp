/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2023
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#include "parameter_intern.h"
#include "parameter_group.h"
#include "streamwriter.h"

namespace rcp {

    IParameter::~IParameter() {}

    template<
        typename T,
        typename TD,
        datatype_t type_id
    >
    std::ostream& operator<<(std::ostream& out, ValueParameter<T, TD, type_id>& param)
    {
        StreamWriter w(out);
        param.write(w);
        return out;
    }
}
