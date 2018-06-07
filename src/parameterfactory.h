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
        static ParameterPtr createParameter(int16_t parameter_id, datatype_t type_id);

        template<typename T>
        static ParameterPtr createRangeParameter(int16_t id, T type) {
            return std::make_shared<RangeParameter<T> >(id);
        }

        static ParameterPtr createRangeParameter(int16_t parameter_id, datatype_t type_id) {

            switch (type_id) {
            case DATATYPE_INT8:
                return RangeParameter<int8_t>::create(parameter_id);

            case DATATYPE_UINT8:
                return RangeParameter<uint8_t>::create(parameter_id);

            case DATATYPE_INT16:
                return RangeParameter<int16_t>::create(parameter_id);

            case DATATYPE_UINT16:
                return RangeParameter<uint16_t>::create(parameter_id);

            case DATATYPE_INT32:
                return RangeParameter<int32_t>::create(parameter_id);

            case DATATYPE_UINT32:
                return RangeParameter<uint32_t>::create(parameter_id);

            case DATATYPE_INT64:
                return RangeParameter<int64_t>::create(parameter_id);

            case DATATYPE_UINT64:
                return RangeParameter<uint64_t>::create(parameter_id);

            case DATATYPE_FLOAT32:
                return RangeParameter<float>::create(parameter_id);

            case DATATYPE_FLOAT64:
                return RangeParameter<double>::create(parameter_id);

            default:
                // error...?
                break;
            }

            return nullptr;
        }
    };

}

#endif
