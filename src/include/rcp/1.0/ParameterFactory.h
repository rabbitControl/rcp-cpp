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

#ifndef RCP_V10_PARAMETERFACTORY_H
#define RCP_V10_PARAMETERFACTORY_H

#include <cstdint>

#include "IParameter.h"
#include "types.h"

namespace rcp {
namespace v10 {

class ParameterFactory
{
public:
    template<typename T>
    static IParameterPtr readValue(const T& p, std::istream& is) {
        p->setValue(p->typeDefinition().parseValue(is));
        return p;
    }

    // template<typename T>
    // static void parse(T& param, std::istream& is) {
    //     param->typeDefinition().parseOptions(is);
    // }

    static IParameterPtr createParameter(int16_t parameter_id, datatype_t type_id);
    static IParameterPtr createParameterReadValue(int16_t parameter_id, datatype_t type_id, std::istream& is);


    // template<typename T,
    //          typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
    // static ParameterPtr createRangeParameter(int16_t id, T /*type*/)
    // {
    //     return std::make_shared<RangeParameter<T> >(id);
    // }

    static IParameterPtr createRangeParameter(int16_t parameter_id, datatype_t type_id);
    // static ParameterPtr createRangeParameterReadValue(int16_t parameter_id, datatype_t type_id, std::istream& is);
};

}
}

#endif // PARAMETERFACTORY_H
