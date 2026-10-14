/*
********************************************************************
* RabbitControl - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of RabbitControl for c++.
*
* Written by Ingo Randolf, 2018-2026
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCP_V10_TYPEDEFINITIONRANGE_H
#define RCP_V10_TYPEDEFINITIONRANGE_H

#include "../TypeDefinitionDefault.h"
#include "../Range.h"
#include "../TypeHelper.h"

#include "TypeDefinitionNumber.h"
#include "types.h"

namespace rcp {
namespace v10 {



template<typename ElementType,
         typename = std::enable_if_t<is_valid_parameter_number_element_v<ElementType>> >
class TypeDefinitionRange
    : public rcp::TypeDefinitionDefault<Range<ElementType>, DATATYPE_RANGE, OptionsDefault>
{
public:
    TypeDefinitionRange(rcp::IParameter* param)
        : rcp::TypeDefinitionDefault<Range<ElementType>, DATATYPE_RANGE, OptionsDefault>(param)
        , element_type(new TypeDefinitionNumber<ElementType>(param))
    {}

    ~TypeDefinitionRange()
    {
        delete element_type;
    }

    TypeDefinitionNumber<ElementType>& elementTypeDefinition() const
    {
        return *element_type;
    }


public:
    virtual bool isDirty() const override
    {
        return rcp::TypeDefinitionDefault<Range<ElementType>, DATATYPE_RANGE, OptionsDefault>::isDirty()
               || element_type->isDirty();
    }

    virtual void parseElementOptions(std::istream& is) override
    {
        element_type->parseOptions(is);
    }

    virtual Range<ElementType> parseValue(std::istream& is) override
    {
        ElementType v1 = readFromStream(is, v1);
        ElementType v2 = readFromStream(is, v2);
        return Range<ElementType>(v1, v2);
    }

    virtual void writeValue(Writer& out, const Range<ElementType>& value) override
    {
        out.write(value.value1());
        out.write(value.value2());
    }

    virtual void write(Writer& out, bool all) override
    {
        out.write(static_cast<char>(this->datatype() | ((all && this->m_defaultValue.hasValue()) || this->m_defaultValue.changed() ? 0x00 : Terminator)));

        element_type->write(out, all);

        if (this->m_defaultValue.hasValue())
        {
            if (all || this->m_defaultValue.changed())
            {
                out.write(static_cast<char>(RANGE_OPTIONS_DEFAULT | Terminator));

                writeValue(out, this->m_defaultValue.value());

                if (!all)
                {
                    this->m_defaultValue.setUnchanged();
                }
            }
        }
        else if (this->m_defaultValue.changed())
        {
            out.write(static_cast<char>(RANGE_OPTIONS_DEFAULT | Terminator));
            writeValue(out, Range<ElementType>{});
            this->m_defaultValue.setUnchanged();
        }
    }

    virtual void writeMandatory(Writer& out) override
    {
        out.write(this->datatype());
        element_type->writeMandatory(out);
    }

    virtual void setAllUnchanged() override
    {
        rcp::TypeDefinitionDefault<Range<ElementType>, DATATYPE_RANGE, OptionsDefault>::setAllUnchanged();
        element_type->setAllUnchanged();
    }


private:
    // all numbers
    TypeDefinitionNumber<ElementType>* element_type{nullptr};

};


}
}

#endif // TYPEDEFINITIONRANGE_H
