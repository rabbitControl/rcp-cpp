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

#ifndef RCP_V10_NUMBERTYPEDEFINITION_H
#define RCP_V10_NUMBERTYPEDEFINITION_H

#include <cstdint>

#include "../TypeDefinitionNumber.h"

#include "RcpString.h"
#include "TypeUtils.h"
#include "types.h"

namespace rcp {
namespace v10 {


template<typename T>
class TypeDefinitionNumber : public rcp::TypeDefinitionNumber<T, convertDatatype<T>::value, OptionsDefault>
{
public:
    TypeDefinitionNumber(rcp::IParameter* param)
        : rcp::TypeDefinitionNumber<T, convertDatatype<T>::value, OptionsDefault>(param)
    {}

    //------------
    // stepsize
    T getStepsize() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        if (stepsize.hasValue())
        {
            return stepsize.value();
        }
        return type_zero(stepsize.value());
    }
    void setStepsize(const T val)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        this->updateOption(stepsize, val);
    }
    bool hasStepsize() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return stepsize.hasValue();
    }
    void clearStepsize()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        this->clearOption(stepsize);
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
        auto write_default = (all && this->m_defaultValue.hasValue()) || this->m_defaultValue.changed();
        auto write_min = (all && this->minimum.hasValue()) || this->minimum.changed();
        auto write_max = (all && this->maximum.hasValue()) || this->maximum.changed();
        auto write_unit = (all && this->unit.hasValue()) || this->unit.changed();
        auto write_stepsize = (all && this->stepsize.hasValue()) || this->stepsize.changed();

        auto all_or = write_default || write_min || write_max || write_unit || write_stepsize;

        out.write(static_cast<char>(this->datatype() | (all_or ? 0x00 : Terminator)));


        all_or = write_min || write_max || write_unit || write_stepsize;

        if (this->m_defaultValue.hasValue())
        {
            if (all || this->m_defaultValue.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_DEFAULT | (all_or ? 0x00 : Terminator)));

                writeValue(out, this->m_defaultValue.value());

                if (!all)
                {
                    this->m_defaultValue.setUnchanged();
                }
            }
        }
        else if (this->m_defaultValue.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_DEFAULT | (all_or ? 0x00 : Terminator)));
            this->writeValue(out, type_zero(T{}));
            this->m_defaultValue.setUnchanged();
        }


        all_or = write_max || write_unit || write_stepsize;

        if (this->minimum.hasValue())
        {
            if (all || this->minimum.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_MINIMUM | (all_or ? 0x00 : Terminator)));

                writeValue(out, this->minimum.value());

                if (!all)
                {
                    this->minimum.setUnchanged();
                }
            }
        }
        else if (this->minimum.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_MINIMUM | (all_or ? 0x00 : Terminator)));
            this->writeValue(out, type_min(T{}));
            this->minimum.setUnchanged();
        }


        all_or = write_unit || write_stepsize;

        if (this->maximum.hasValue())
        {
            if (all || this->maximum.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_MAXIMUM | (all_or ? 0x00 : Terminator)));

                writeValue(out, this->maximum.value());

                if (!all)
                {
                    this->maximum.setUnchanged();
                }
            }
        }
        else if (this->maximum.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_MAXIMUM | (all_or ? 0x00 : Terminator)));
            this->writeValue(out, type_max(T{}));
            this->maximum.setUnchanged();
        }


        all_or = write_stepsize;

        if (this->unit.hasValue())
        {
            if (all || this->unit.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_UNIT | (all_or ? 0x00 : Terminator)));

                RcpString(this->unit.value()).write(out, true);

                if (!all)
                {
                    this->unit.setUnchanged();
                }
            }
        }
        else if (this->unit.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_UNIT | (all_or ? 0x00 : Terminator)));
            RcpString("").write(out, true);
            this->unit.setUnchanged();
        }


        if (this->stepsize.hasValue())
        {
            if (all || this->stepsize.changed())
            {
                out.write(static_cast<char>(NUMBER_OPTIONS_STEPSIZE | Terminator));

                writeValue(out, stepsize.value());

                if (!all)
                {
                    this->stepsize.setUnchanged();
                }
            }
        }
        else if (this->stepsize.changed())
        {
            out.write(static_cast<char>(NUMBER_OPTIONS_STEPSIZE | Terminator));
            writeValue(out, type_zero(T{}));
            this->stepsize.setUnchanged();
        }

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
        case NUMBER_OPTIONS_STEPSIZE: {

            T stepsize = rcp::readFromStream(is, stepsize);
            CHECK_STREAM;

            this->stepsize = stepsize;
            return true;
        }
        case NUMBER_OPTIONS_UNIT: {

            std::string unit = RcpString::parse(is);
            CHECK_STREAM;

            this->unit = unit;
            return true;
        }
        }

        return rcp::TypeDefinitionNumber<T, convertDatatype<T>::value, OptionsDefault>::handleOption(optionId, is);
    }



private:
    Option<T> stepsize;

};

}
}


#endif // NUMBERTYPEDEFINITION_H
