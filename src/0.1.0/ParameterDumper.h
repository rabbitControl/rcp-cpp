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

#ifndef RCP_V010_PARAMETERDUMPER_H
#define RCP_V010_PARAMETERDUMPER_H

#include "../IParameter.h"

namespace rcp {
namespace v010 {

class ParameterDumper
{
public:
    static void dump(IParameterPtr param);

    template<typename T>
    static void dumpValue(const T& v_param)
    {
        if (v_param)
        {
            std::cout << "\tvalue: " << v_param->getValue() << std::endl;
        }
    }
};

}
}

#endif // PARAMETERDUMPER_H
