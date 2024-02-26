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

#ifndef RCP_PARAMETERFACTORY_H
#define RCP_PARAMETERFACTORY_H

#include <inttypes.h>

#include "types.h"
#include "iparameter.h"
#include "parameter_range.h"
#include "parameter_custom.h"

namespace rcp {

    class ParameterFactory
    {
    public:
        template<typename T>
        static ParameterPtr readValue(const T& p, std::istream& is) {
            p->setValue(p->getDefaultTypeDefinition().readValue(is));
            return p;
        }

        static ParameterPtr createParameter(int16_t parameter_id, datatype_t type_id);
        static ParameterPtr createParameterReadValue(int16_t parameter_id, datatype_t type_id, std::istream& is);


        template<typename T>
        static ParameterPtr createRangeParameter(int16_t id, T /*type*/) {
            return std::make_shared<RangeParameter<T> >(id);
        }

        static ParameterPtr createRangeParameter(int16_t parameter_id, datatype_t type_id);
        static ParameterPtr createRangeParameterReadValue(int16_t parameter_id, datatype_t type_id, std::istream& is);
    };

}

#endif
