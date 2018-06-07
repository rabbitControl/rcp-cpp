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
