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

#ifndef RCP_TYPEDEFINITION_H
#define RCP_TYPEDEFINITION_H

#include <string>

#include "types.h"
#include "writeable.h"
#include "optionparser.h"

namespace rcp {

    static const std::string empty_string = "";

    class ITypeDefinition : public virtual Writeable, public IOptionparser
    {
    public:
        virtual datatype_t getDatatype() const = 0;
        virtual void dump() = 0;
        virtual void writeMandatory(Writer& out) const = 0;

        virtual bool anyOptionChanged() const = 0;
    };


    template<typename T>
    class IDefaultDefinition : public virtual ITypeDefinition
    {
    public:
        virtual const T getDefault() const = 0;
        virtual void setDefault(const T& defaultValue) = 0;
        virtual bool hasDefault() const = 0;
        virtual void clearDefault() = 0;

        virtual T readValue(std::istream& is) = 0;
    };


    template<typename T>
    class INumberDefinition : public virtual IDefaultDefinition<T>
    {
    public:
        virtual T getMinimum() const = 0;
        virtual void setMinimum(const T& min) = 0;
        virtual bool hasMinimum() const = 0;
        virtual void clearMinimum() = 0;

        virtual T getMaximum() const = 0;
        virtual void setMaximum(const T& min) = 0;
        virtual bool hasMaximum() const = 0;
        virtual void clearMaximum() = 0;

        virtual T getMultipleof() const = 0;
        virtual void setMultipleof(const T& min) = 0;
        virtual bool hasMultipleof() const = 0;
        virtual void clearMultipleof() = 0;

        virtual number_scale_t getScale() const = 0;
        virtual void setScale(const number_scale_t& scale) = 0;
        virtual bool hasScale() const = 0;
        virtual void clearScale() = 0;

        virtual std::string getUnit() const = 0;
        virtual void setUnit(const std::string& unit) = 0;
        virtual bool hasUnit() const = 0;
        virtual void clearUnit() = 0;
    };


    /*
    Value Typedefinition class
    with partial type specialisation
    */
    template<
            typename T,
            datatype_t type_id,
            td_types t
    >
    class TypeDefinition;


    //
    template<typename T,
             datatype_t type_id,
             td_types t
    >
    std::ostream& operator<<(std::ostream& out, rcp::TypeDefinition<T, type_id, t>& type);
}

#endif // RCP_TYPEDEFINITION_H
