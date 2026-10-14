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

#ifndef RCP_TYPEDEFINITIONNUMBER_H
#define RCP_TYPEDEFINITIONNUMBER_H

#include <cstdint>

#include "TypeDefinitionDefault.h"
#include "TypeHelper.h"

namespace rcp
{

template<typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
T type_min(T)
{
    return std::numeric_limits<T>::min();
}

template<typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
T type_max(T)
{
    return std::numeric_limits<T>::max();
}

template<typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
T type_zero(T)
{
    return 0;
}



template<typename T,
         uint8_t type,
         uint8_t defaultprefix,
         typename = typename std::enable_if_t<is_valid_parameter_number_element_v<T>> >
class TypeDefinitionNumber
    : public TypeDefinitionDefault<T, type, defaultprefix>
{
public:
    TypeDefinitionNumber(IParameter* param) : TypeDefinitionDefault<T, type, defaultprefix>(param) {}





    //------------------------------------
    // implement INumberDefinition

    //--------
    // minimum
    T getMinimum() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        if (minimum.hasValue())
        {
            return minimum.value();
        }
        return type_zero(minimum.value());
    }
    void setMinimum(const T val)
    {
        this->updateOption(minimum, val);
    }
    bool hasMinimum() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return minimum.hasValue();
    }
    void clearMinimum()
    {
        this->clearOption(minimum);
    }

    //--------
    // maximum
    T getMaximum() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        if (maximum.hasValue())
        {
            return maximum.value();
        }
        return type_zero(maximum.value());
    }
    void setMaximum(const T val)
    {
        this->updateOption(maximum, val);
    }
    bool hasMaximum() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return maximum.hasValue();
    }
    void clearMaximum()
    {
        this->clearOption(maximum);
    }



    //-----
    // unit
    std::string getUnit() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return unit.value();
    }
    void setUnit(const std::string& val)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        this->updateOption(unit, val);
    }
    bool hasUnit() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return unit.hasValue();
    }
    void clearUnit()
    {
        this->clearOption(unit);
    }


    virtual void update(const TypeDefinitionNumber& other)
    {
        TypeDefinitionDefault<T, type, defaultprefix>::update(other);

        minimum = other.minimum;
        maximum = other.maximum;
    }

    virtual void setAllUnchanged() override
    {
        TypeDefinitionDefault<T, type, defaultprefix>::setAllUnchanged();
        minimum.setUnchanged();
        maximum.setUnchanged();
        unit.setUnchanged();
    }


protected:
    Option<T> minimum;
    Option<T> maximum;
    Option<std::string> unit;

};

}


#endif // TYPEDEFINITIONNUMBER_H
