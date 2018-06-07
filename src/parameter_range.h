#ifndef RCP_PARAMETER_RANGE_H
#define RCP_PARAMETER_RANGE_H

#include <vector>

#include "parameter_intern.h"
#include "type_range.h"

namespace rcp {

    template <typename ElementType>
    class RangeParameter :
            public ValueParameter<Range<ElementType>, TypeDefinition<Range<ElementType>, DATATYPE_RANGE, td_num>, DATATYPE_RANGE>
            , public IElementParameter
    {
    public:
        typedef TypeDefinition<Range<ElementType>, DATATYPE_RANGE, td_num> _RangeType;
        typedef ValueParameter<Range<ElementType>, TypeDefinition<Range<ElementType>, DATATYPE_RANGE, td_num>, DATATYPE_RANGE> _RangeParameter;

        static ParameterPtr create(int16_t id) {
            return std::make_shared<RangeParameter<ElementType> >(id);
        }

        RangeParameter(int16_t id) :
            _RangeParameter(id)
        {}

        ~RangeParameter()
        {}

        // IElementParameter
        virtual datatype_t getElementType() {
            return _RangeParameter::getDefaultTypeDefinition().getElementType().getDatatype();
        }

        // convenience
        void setDefault(const Range<ElementType>& v) {
            _RangeParameter::getDefaultTypeDefinition().setDefault(v);
        }
        void setElementDefault(const ElementType& v) {
            _RangeParameter::getDefaultTypeDefinition().getElementType().setDefault(v);
        }
        void setMinimum(const ElementType& v) {
            _RangeParameter::getDefaultTypeDefinition().setMinimum(v);
        }
        void setMaximum(const ElementType& v) {
            _RangeParameter::getDefaultTypeDefinition().setMaximum(v);
        }
        void setMultipleof(const ElementType& v) {
            _RangeParameter::getDefaultTypeDefinition().setMultipleof(v);
        }
        void setScale(const number_scale_t& v) {
            _RangeParameter::getDefaultTypeDefinition().setScale(v);
        }
        void setUnit(const std::string& v) {
            _RangeParameter::getDefaultTypeDefinition().setUnit(v);
        }

        virtual void dump() {
            Parameter<_RangeType>::dump();

            if (_RangeParameter::hasValue()) {
                Range<ElementType> r = _RangeParameter::getValue();
                std::cout << "value: " << r.value1() << ":" << r.value2() << "\n";
            }
        }
    };
}

#endif
