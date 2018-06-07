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

        static ParameterPtr createRangeParameter(int16_t parameter_id, datatype_t type_id);
    };

}

#endif
