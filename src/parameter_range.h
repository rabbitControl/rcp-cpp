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

#ifndef RCP_PARAMETER_RANGE_H
#define RCP_PARAMETER_RANGE_H

#include <vector>

#include "parameter_intern.h"
#include "range.h"

namespace rcp {

    template <typename ElementType>
    class RangeParameter
        : public ValueParameter<Range<ElementType>, TypeDefinition<Range<ElementType>, DATATYPE_RANGE, td_num>, DATATYPE_RANGE>
        , public IElementParameter
    {
    public:

        typedef TypeDefinition<Range<ElementType>, DATATYPE_RANGE, td_num> _RangeType;
        typedef ValueParameter<Range<ElementType>, TypeDefinition<Range<ElementType>, DATATYPE_RANGE, td_num>, DATATYPE_RANGE> _RangeParameter;

        static std::shared_ptr< _RangeParameter > create(int16_t id) {
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
            }
        }
    };
}

#endif
