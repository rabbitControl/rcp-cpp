#include "typedefinition.h"
#include "streamwriter.h"

#include "type_enum.h"
#include "type_string.h"
#include "type_uri.h"

namespace rcp {

    ITypeDefinition::~ITypeDefinition() {}

    TypeDefinition<std::string, DATATYPE_ENUM, td_enum>::~TypeDefinition<std::string, DATATYPE_ENUM, td_enum>() {}
    TypeDefinition<std::string, DATATYPE_STRING, td_string>::~TypeDefinition<std::string, DATATYPE_STRING, td_string>() {}
    TypeDefinition<std::string, DATATYPE_URI, td_uri>::~TypeDefinition<std::string, DATATYPE_URI, td_uri>() {}

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
