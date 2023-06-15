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

#ifndef RCP_PARAMETERPARSER_H
#define RCP_PARAMETERPARSER_H

#include "specializetypes.h"
#include "parameterfactory.h"


namespace rcp {

    class ParameterParser {
    public:

        static ParameterPtr parseUpdateValue(std::istream& is) {

            // read id
            int16_t parameter_id = 0;
            parameter_id = readFromStream(is, parameter_id);

            // get parameter type_id
            datatype_t type_id = static_cast<datatype_t>(is.get());

            if (type_id == DATATYPE_BANG || type_id == DATATYPE_GROUP) {
                return nullptr;
            }

            return ParameterFactory::createParameterReadValue(parameter_id, type_id, is);
        }

        static ParameterPtr parse(std::istream& is, std::shared_ptr<IParameterManager> manager = nullptr) {

            // get id and type            
            int16_t parameter_id = 0;
            parameter_id = readFromStream(is, parameter_id);

            if (is.eof()) {
                return nullptr;
            }

            // get parameter type_id
            datatype_t type_id = static_cast<datatype_t>(is.get());

            // validity check
            if (is.eof() ||
                type_id < 0 ||
                type_id >= DATATYPE_MAX_)
            {
                // fail
                return nullptr;
            }

            //
            ParameterPtr param;

            // handle certain datatypes
            if (type_id == DATATYPE_RANGE) {

                // get element type
                datatype_t element_type_id = static_cast<datatype_t>(is.get());

                param = ParameterFactory::createRangeParameter(parameter_id, element_type_id);
                if (!param) {
                    std::cerr << "could not create rangeparameter with element_type: " << element_type_id << "\n";
                    return nullptr;
                }

                param->getTypeDefinition().parseOptions(is);

            } else if (type_id == DATATYPE_ARRAY) {
                // TODO
            } else if (type_id == DATATYPE_LIST) {
                // TODO
            } else {

                param = ParameterFactory::createParameter(parameter_id, type_id);

                if (!param) {
                    return nullptr;
                }

                param->getTypeDefinition().parseOptions(is);
            }

            if (param) {
                param->setManager(manager);
                param->parseOptions(is);
            }

            return param;
        }
    };

}

#endif
