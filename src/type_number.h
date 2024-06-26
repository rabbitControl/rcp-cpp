/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2024
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef NUMBERTYPE_H
#define NUMBERTYPE_H

#include <iostream>
#include <limits>

#include "typedefinition.h"
#include "iparameter.h"
#include "stream_tools.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

namespace rcp {


template<typename T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
T type_min(T)
{
    return std::numeric_limits<T>::min();
}

template<typename T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
T type_max(T)
{
    return std::numeric_limits<T>::max();
}

template<typename T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
T type_zero(T)
{
    return 0;
}

template<typename T>
std::string value_to_string(T value)
{
    return std::to_string(value);
}


template<
    typename T,
    datatype_t type_id
    >
class TypeDefinition<T, type_id, td_num >
    : public INumberDefinition<T>
{
public:
    TypeDefinition(TypeDefinition<T, type_id, td_num >& v) :
        obj(v.obj)
    {}

    TypeDefinition(const TypeDefinition<T, type_id, td_num >& v) :
        obj(v.obj)
    {}

    TypeDefinition(IParameter& param) :
        obj(std::make_shared<Value>(param))
    {}

    TypeDefinition(const T& dv, IParameter& param) :
        obj(std::make_shared<Value>(dv, param))
    {}

    TypeDefinition(const T& dv, const T& min, const T& max, IParameter& param) :
        obj(std::make_shared<Value>(dv, min, max, param))
    {}

    //------------------------------------
    // Writeable
    void write(Writer& out, bool all) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->write(out, all);

        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }

    //------------------------------------
    // ITypeDefinition
    void writeMandatory(Writer& out) const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        obj->writeMandatory(out);
    }

    datatype_t getDatatype() const override
    {
        return obj->datatype;
    }

    bool anyOptionChanged() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        return obj->defaultValue.changed()
               || obj->minimum.changed()
               || obj->maximum.changed()
               || obj->multipleof.changed()
               || obj->scale.changed()
               || obj->unit.changed();
    }

    void dump() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        std::cout << "--- type number ---\n";

        if (hasDefault()) {
            std::cout << "\tdefault: " << value_to_string(getDefault()) << "\n";
        }

        if (hasMinimum()) {
            std::cout << "\tminimum: " << value_to_string(getMinimum()) << "\n";
        }

        if (hasMaximum()) {
            std::cout << "\tmaximum: " << value_to_string(getMaximum()) << "\n";
        }

        if (hasMultipleof()) {
            std::cout << "\tmultipleof: " << value_to_string(getMultipleof()) << "\n";
        }

        if (hasScale()) {
            std::cout << "\tscale: " << getScale() << "\n";
        }

        if (hasUnit()) {
            std::cout << "\tunit: " << getUnit() << "\n";
        }
    }

    //------------------------------------
    // IOptionparser
    void parseOptions(std::istream& is) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        while (!is.eof()) {

            // read option prefix
            number_options_t opt = static_cast<number_options_t>(is.get());

            CHECK_STREAM_MSG("typedefinition - could not read from stream")

            if (opt == TERMINATOR) {
                break;
            }

            switch (opt) {
            case NUMBER_OPTIONS_DEFAULT: {

                // read options
                T def = readFromStream(is, def);
                CHECK_STREAM;

                obj->defaultValue = def;
                break;
            }
            case NUMBER_OPTIONS_MINIMUM: {

                T min = readFromStream(is, min);
                CHECK_STREAM;

                obj->minimum = min;
                break;
            }
            case NUMBER_OPTIONS_MAXIMUM: {

                T max = readFromStream(is, max);
                CHECK_STREAM;

                obj->maximum = max;
                break;
            }
            case NUMBER_OPTIONS_MULTIPLEOF: {

                T mult = readFromStream(is, mult);
                CHECK_STREAM;

                obj->multipleof = mult;
                break;
            }
            case NUMBER_OPTIONS_SCALE: {

                number_scale_t scale = static_cast<number_scale_t>(is.get());
                CHECK_STREAM;

                obj->scale = scale;
                break;
            }
            case NUMBER_OPTIONS_UNIT: {

                std::string unit = readTinyString(is);
                CHECK_STREAM;

                obj->unit = unit;
                break;
            }
            }

        }
    } // parseOptions


    //------------------------------------
    // IDefaultDefinition
    T readValue(std::istream& is) override
    {
        T val = readFromStream(is, val);
        return val;
    }

    // default value
    const T getDefault() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        if (obj->defaultValue.hasValue())
        {
            return obj->defaultValue.value();
        }
        return type_zero(obj->defaultValue.value());
    }
    void setDefault(const T& defaultValue) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->defaultValue = defaultValue;
        if (obj->defaultValue.changed())
        {
            setDirty();
        }
    }
    bool hasDefault() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->defaultValue.hasValue();
    }
    void clearDefault() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->defaultValue.clearValue();
        if (obj->defaultValue.changed())
        {
            setDirty();
        }
    }

    //------------------------------------
    // implement INumberDefinition

    //--------
    // minimum
    T getMinimum() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        if (obj->minimum.hasValue())
        {
            return obj->minimum.value();
        }
        return type_zero(obj->minimum.value());
    }
    void setMinimum(const T val) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->minimum = val;
        if (obj->minimum.changed())
        {
            setDirty();
        }
    }
    bool hasMinimum() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->minimum.hasValue();
    }
    void clearMinimum() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        obj->minimum.clearValue();
        if (obj->minimum.changed())
        {
            setDirty();
        }
    }

    //--------
    // maximum
    T getMaximum() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        if (obj->maximum.hasValue())
        {
            return obj->maximum.value();
        }
        return type_zero(obj->maximum.value());
    }
    void setMaximum(const T val) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->maximum = val;
        if (obj->maximum.changed())
        {
            setDirty();
        }
    }
    bool hasMaximum() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->maximum.hasValue();
    }
    void clearMaximum() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->maximum.clearValue();
        if (obj->maximum.changed())
        {
            setDirty();
        }
    }

    //------------
    // multiple of
    T getMultipleof() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        if (obj->multipleof.hasValue())
        {
            return obj->multipleof.value();
        }
        return type_zero(obj->multipleof.value());
    }
    void setMultipleof(const T val) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->multipleof = val;
        if (obj->multipleof.changed())
        {
            setDirty();
        }
    }
    bool hasMultipleof() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->multipleof.hasValue();
    }
    void clearMultipleof() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->multipleof.clearValue();
        if (obj->multipleof.changed())
        {
            setDirty();
        }
    }

    //------
    // scale
    number_scale_t getScale() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        if (obj->scale.hasValue())
        {
            return obj->scale.value();
        }
        return NUMBER_SCALE_LINEAR;
    }
    void setScale(const number_scale_t val) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->scale = val;
        if (obj->scale.changed())
        {
            setDirty();
        }
    }
    bool hasScale() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->scale.hasValue();
    }
    void clearScale() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->scale.clearValue();
        if (obj->scale.changed())
        {
            setDirty();
        }
    }

    //-----
    // unit
    std::string getUnit() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->unit.value();
    }
    void setUnit(const std::string& val) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->unit = val;
        if (obj->unit.changed())
        {
            setDirty();
        }
    }
    bool hasUnit() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->unit.hasValue();
    }
    void clearUnit() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->unit.clearValue();
        if (obj->unit.changed())
        {
            setDirty();
        }
    }


    void setAllUnchanged() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->defaultValue.setUnchanged();
        obj->minimum.setUnchanged();
        obj->maximum.setUnchanged();
        obj->multipleof.setUnchanged();
        obj->scale.setUnchanged();
        obj->unit.setUnchanged();
    }

private:
    void setDirty()
    {
        obj->parameter.setDirty();
    }

    class Value {
    public:
        Value(IParameter& param) :
            datatype(type_id)
            , parameter(param)
        {}

        Value(const T& defaultValue, IParameter& param) :
            datatype(type_id)
            , defaultValue(defaultValue)
            , parameter(param)
        {}

        Value(const T& defaultValue, const T& min, const T& max, IParameter& param) :
            datatype(type_id)
            , defaultValue(defaultValue)
            , minimum(min)
            , maximum(max)
            , parameter(param)
        {}

        void writeMandatory(Writer& out) {
            out.write(static_cast<char>(datatype));
        }

        virtual void write(Writer& out, bool all) {

            writeMandatory(out);

            // write default value
            if (defaultValue.hasValue()) {

                if (all || defaultValue.changed()) {
                    out.write(static_cast<char>(NUMBER_OPTIONS_DEFAULT));
                    out.write(defaultValue.value());

                    if (!all) {
                        defaultValue.setUnchanged();
                    }
                }
            } else if (defaultValue.changed()) {
                out.write(static_cast<char>(NUMBER_OPTIONS_DEFAULT));
                out.write(type_zero(defaultValue.value()));
                defaultValue.setUnchanged();
            }


            // minimum
            if (minimum.hasValue()) {

                if (all || minimum.changed()) {
                    out.write(static_cast<char>(NUMBER_OPTIONS_MINIMUM));
                    out.write(minimum.value());

                    if (!all) {
                        minimum.setUnchanged();
                    }
                }
            } else if (minimum.changed()) {

                out.write(static_cast<char>(NUMBER_OPTIONS_MINIMUM));
                out.write(type_min(minimum.value()));
                minimum.setUnchanged();
            }


            // maximum
            if (maximum.hasValue()) {

                if (all || maximum.changed()) {
                    out.write(static_cast<char>(NUMBER_OPTIONS_MAXIMUM));
                    out.write(maximum.value());

                    if (!all) {
                        maximum.setUnchanged();
                    }
                }
            } else if (maximum.changed()) {

                out.write(static_cast<char>(NUMBER_OPTIONS_MAXIMUM));
                out.write(type_max(maximum.value()));
                maximum.setUnchanged();
            }


            // multipleof
            if (multipleof.hasValue()) {

                if (all || multipleof.changed()) {
                    out.write(static_cast<char>(NUMBER_OPTIONS_MULTIPLEOF));
                    out.write(multipleof.value());

                    if (!all) {
                        multipleof.setUnchanged();
                    }
                }
            } else if (multipleof.changed()) {

                out.write(static_cast<char>(NUMBER_OPTIONS_MULTIPLEOF));
                out.write(static_cast<T>(type_zero(multipleof.value())));
                multipleof.setUnchanged();
            }


            // scale
            if (scale.hasValue()) {

                if (all || scale.changed()) {
                    out.write(static_cast<char>(NUMBER_OPTIONS_SCALE));
                    out.write(static_cast<char>(scale.value()));

                    if (!all) {
                        scale.setUnchanged();
                    }
                }
            } else if (scale.changed()) {

                out.write(static_cast<char>(NUMBER_OPTIONS_SCALE));
                out.write(static_cast<char>(NUMBER_SCALE_LINEAR));
                scale.setUnchanged();
            }


            // unit
            if (unit.hasValue()) {

                if (all || unit.changed()) {
                    out.write(static_cast<char>(NUMBER_OPTIONS_UNIT));
                    out.writeTinyString(unit.value());

                    if (!all) {
                        unit.setUnchanged();
                    }
                }
            } else if (unit.changed()) {

                out.write(static_cast<char>(NUMBER_OPTIONS_UNIT));
                out.writeTinyString("");
                unit.setUnchanged();
            }
        }

        // mandatory
        datatype_t datatype;

        // options - base
        Option<T> defaultValue;

        // options - number
        Option<T> minimum;
        Option<T> maximum;
        Option<T> multipleof;

        Option<number_scale_t> scale;
        Option<std::string> unit;

        IParameter& parameter;
    };
    std::shared_ptr<Value> obj;
    TypeDefinition(std::shared_ptr<Value> obj) :obj(obj) {}
};

//
typedef TypeDefinition<int8_t, DATATYPE_INT8, td_num > Int8TypeDefinition;
typedef TypeDefinition<uint8_t, DATATYPE_UINT8, td_num > UInt8TypeDefinition;
typedef TypeDefinition<int16_t, DATATYPE_INT16, td_num > Int16TypeDefinition;
typedef TypeDefinition<uint16_t, DATATYPE_UINT16, td_num > UInt16TypeDefinition;
typedef TypeDefinition<int32_t, DATATYPE_INT32, td_num > Int32TypeDefinition;
typedef TypeDefinition<uint32_t, DATATYPE_UINT32, td_num > UInt32TypeDefinition;
typedef TypeDefinition<int64_t, DATATYPE_INT64, td_num > Int64TypeDefinition;
typedef TypeDefinition<uint64_t, DATATYPE_UINT64, td_num > UInt64TypeDefinition;
typedef TypeDefinition<float, DATATYPE_FLOAT32, td_num > Float32TypeDefinition;
typedef TypeDefinition<double, DATATYPE_FLOAT64, td_num > Float64TypeDefinition;

// vector
typedef TypeDefinition<Vector2<int>, DATATYPE_VECTOR2I32, td_num > Vector2I32TypeDefinition;
typedef TypeDefinition<Vector2<float>, DATATYPE_VECTOR2F32, td_num > Vector2F32TypeDefinition;
typedef TypeDefinition<Vector3<int>, DATATYPE_VECTOR3I32, td_num > Vector3I32TypeDefinition;
typedef TypeDefinition<Vector3<float>, DATATYPE_VECTOR3F32, td_num > Vector3F32TypeDefinition;
typedef TypeDefinition<Vector4<int>, DATATYPE_VECTOR4I32, td_num > Vector4I32TypeDefinition;
typedef TypeDefinition<Vector4<float>, DATATYPE_VECTOR4F32, td_num > Vector4F32TypeDefinition;
}

#endif // NUMBERTYPE_H
