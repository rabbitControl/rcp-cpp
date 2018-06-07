#ifndef RCP_PARAMETER_CUSTOM_H
#define RCP_PARAMETER_CUSTOM_H

#include "parameter_intern.h"

namespace rcp {

    template <typename T>
    class CustomParameter :
            public ValueParameter<T, TypeDefinition<T, DATATYPE_CUSTOMTYPE, td_custom>, DATATYPE_CUSTOMTYPE>
    {
    public:
        typedef TypeDefinition<T, DATATYPE_CUSTOMTYPE, td_custom> _CustomType;
        typedef ValueParameter<T, TypeDefinition<T, DATATYPE_CUSTOMTYPE, td_custom>, DATATYPE_CUSTOMTYPE> _CustomParameter;

        static ParameterPtr create(int16_t id) {
            return std::make_shared<CustomParameter<T> >(id);
        }

        CustomParameter(int16_t id) :
            _CustomParameter(id)
        {}

        ~CustomParameter()
        {}

        // convenience
        void setDefault(const T& v) {
            _CustomParameter::getDefaultTypeDefinition().setDefault(v);
        }

        void setUuid(char* uuid, uint8_t length) {
            _CustomParameter::getDefaultTypeDefinition().setUuid(uuid, length);
        }

        void setConfig(const std::vector<int8_t>& config) {
            _CustomParameter::getDefaultTypeDefinition().setConfig(config);
        }
    };
}

#endif
