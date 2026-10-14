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

#ifndef RCP_V010_NUMBERTYPEDEFINITION_H
#define RCP_V010_NUMBERTYPEDEFINITION_H

#include <cstdint>

#include "../TypeDefinitionNumber.h"
#include "../StreamTools.h"

#include "types.h"
#include "TypeUtils.h"

namespace rcp {
namespace v010 {


template<typename T>
class TypeDefinitionNumber
    : public rcp::TypeDefinitionNumber<T, convertDatatype<T>::value, OptionsDefault>
{
public:
    TypeDefinitionNumber(IParameter* param)
        : rcp::TypeDefinitionNumber<T, convertDatatype<T>::value, OptionsDefault>(param)
    {}

    //------------
    // multiple of
    T getMultipleof() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        if (multipleof.hasValue())
        {
            return multipleof.value();
        }
        return type_zero(multipleof.value());
    }
    void setMultipleof(const T val)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        this->updateOption(multipleof, val);
    }
    bool hasMultipleof() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return multipleof.hasValue();
    }
    void clearMultipleof()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        this->clearOption(multipleof);
    }

    //------
    // scale
    number_scale_t getScale() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        if (scale.hasValue())
        {
            return scale.value();
        }
        return NUMBER_SCALE_LINEAR;
    }
    void setScale(const number_scale_t val)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        this->updateOption(scale, val);
    }
    bool hasScale() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return scale.hasValue();
    }
    void clearScale()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        this->clearOption(scale);
    }


    virtual void update(const TypeDefinitionNumber& other)
    {
        rcp::TypeDefinitionNumber<T, convertDatatype<T>::value, OptionsDefault>::update(other);

        multipleof = other.multipleof;
        scale = other.scale;
    }




    virtual T parseValue(std::istream& is) override
    {
        return readFromStream(is, T{});
    }

    virtual void writeValue(Writer& out, const T& value) override
    {
        out.write(value);
    }

    virtual void write(Writer& out, bool all) override
    {
        out.write(this->datatype());


        // write default value
        if (this->m_defaultValue.hasValue())
        {
            if (all || this->m_defaultValue.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_DEFAULT));

                writeValue(out, this->m_defaultValue.value());

                if (!all)
                {
                    this->m_defaultValue.setUnchanged();
                }
            }
        }
        else if (this->m_defaultValue.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_DEFAULT));
            out.write(type_zero(this->m_defaultValue.value()));
            this->m_defaultValue.setUnchanged();
        }


        // minimum
        if (this->minimum.hasValue())
        {
            if (all || this->minimum.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_MINIMUM));

                writeValue(out, this->minimum.value());

                if (!all) {
                    this->minimum.setUnchanged();
                }
            }
        }
        else if (this->minimum.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_MINIMUM));
            out.write(type_min(this->minimum.value()));
            this->minimum.setUnchanged();
        }


        // maximum
        if (this->maximum.hasValue())
        {
            if (all || this->maximum.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_MAXIMUM));

                writeValue(out, this->maximum.value());

                if (!all)
                {
                    this->maximum.setUnchanged();
                }
            }
        }
        else if (this->maximum.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_MAXIMUM));
            out.write(type_max(this->maximum.value()));
            this->maximum.setUnchanged();
        }


        // multipleof
        if (multipleof.hasValue())
        {
            if (all || multipleof.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_MULTIPLEOF));

                writeValue(out, multipleof.value());

                if (!all)
                {
                    multipleof.setUnchanged();
                }
            }
        }
        else if (multipleof.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_MULTIPLEOF));
            out.write(type_zero(multipleof.value()));
            multipleof.setUnchanged();
        }


        // scale
        if (scale.hasValue())
        {
            if (all || scale.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_SCALE));

                out.write(static_cast<char>(scale.value()));

                if (!all)
                {
                    scale.setUnchanged();
                }
            }
        }
        else if (scale.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_SCALE));
            out.write(static_cast<char>(NUMBER_SCALE_LINEAR));
            scale.setUnchanged();
        }


        // unit
        if (this->unit.hasValue())
        {
            if (all || this->unit.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_UNIT));
                ParameterWriter::writeTinyString(out, this->unit.value());

                if (!all)
                {
                    this->unit.setUnchanged();
                }
            }
        }
        else if (this->unit.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_UNIT));
            ParameterWriter::writeTinyString(out, "");
            this->unit.setUnchanged();
        }


        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }

    virtual bool handleOption(uint8_t optionId, std::istream& is) override
    {
        switch (optionId) {

        case NUMBER_OPTIONS_MINIMUM: {

            T min = rcp::readFromStream(is, min);
            CHECK_STREAM;

            this->minimum = min;
            return true;
        }
        case NUMBER_OPTIONS_MAXIMUM: {

            T max = rcp::readFromStream(is, max);
            CHECK_STREAM;

            this->maximum = max;
            return true;
        }
        case NUMBER_OPTIONS_MULTIPLEOF: {

            T mult = rcp::readFromStream(is, mult);
            CHECK_STREAM;

            this->multipleof = mult;
            return true;
        }
        case NUMBER_OPTIONS_SCALE: {

            number_scale_t scale = static_cast<number_scale_t>(is.get());
            CHECK_STREAM;

            this->scale = scale;
            return true;
        }
        case NUMBER_OPTIONS_UNIT: {

            std::string unit = ParameterParser::readTinyString(is);
            CHECK_STREAM;

            this->unit = unit;
            return true;
        }
        }

        return rcp::TypeDefinitionNumber<T, convertDatatype<T>::value, OptionsDefault>::handleOption(optionId, is);
    }


    virtual void setAllUnchanged() override
    {
        rcp::TypeDefinitionNumber<T, convertDatatype<T>::value, OptionsDefault>::setAllUnchanged();
        multipleof.setUnchanged();
        scale.setUnchanged();
    }


private:
    Option<T> multipleof;

    Option<number_scale_t> scale;

};

}
}


#endif // NUMBERTYPEDEFINITION_H
