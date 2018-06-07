#ifndef RCP_PARAMETERPARSER_H
#define RCP_PARAMETERPARSER_H

#include "specializetypes.h"
#include "parameterfactory.h"

namespace rcp {

    class ParameterParser {
    public:
        static ParameterPtr parse(std::istream& is) {

            // get id and type
            int16_t parameter_id = readFromStream(is, parameter_id);

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

                // get subtype
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
                param->parseOptions(is);
            }

            return param;
        }
    };

}

#endif
