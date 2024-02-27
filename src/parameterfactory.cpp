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

#include "parameterfactory.h"

#include "parameter_range.h"
#include "parameter_custom.h"

namespace rcp {

ParameterPtr ParameterFactory::createParameter(int16_t parameter_id, datatype_t type_id)
{
    switch (type_id) {
    case DATATYPE_BOOLEAN:
        return BooleanParameter::create(parameter_id, false);

    case DATATYPE_INT8:
        return Int8Parameter::create(parameter_id, 0);

    case DATATYPE_UINT8:
        return UInt8Parameter::create(parameter_id, 0);

    case DATATYPE_INT16:
        return Int16Parameter::create(parameter_id, 0);

    case DATATYPE_UINT16:
        return UInt16Parameter::create(parameter_id, 0);

    case DATATYPE_INT32:
        return Int32Parameter::create(parameter_id, 0);

    case DATATYPE_UINT32:
        return UInt32Parameter::create(parameter_id, 0);

    case DATATYPE_INT64:
        return Int64Parameter::create(parameter_id, 0);

    case DATATYPE_UINT64:
        return UInt64Parameter::create(parameter_id, 0);

    case DATATYPE_FLOAT32:
        return Float32Parameter::create(parameter_id, 0);

    case DATATYPE_FLOAT64:
        return Float64Parameter::create(parameter_id, 0);

    case DATATYPE_STRING:
        return StringParameter::create(parameter_id);


    case DATATYPE_ENUM:
        return EnumParameter::create(parameter_id);


    case DATATYPE_RGB:
        return RGBParameter::create(parameter_id);

    case DATATYPE_RGBA:
        return RGBAParameter::create(parameter_id);


    case DATATYPE_URI:
        return URIParameter::create(parameter_id);

    case DATATYPE_IPV4:
        return IPv4Parameter::create(parameter_id);

    case DATATYPE_IPV6:
        return IPv6Parameter::create(parameter_id);


    case DATATYPE_BANG:
        return BangParameter::create(parameter_id);

    case DATATYPE_GROUP:
        return GroupParameter::create(parameter_id);


    case DATATYPE_RANGE:
        //return RangeParameter::create(parameter_id);
        break;

    case DATATYPE_VECTOR2I32:
        return Vector2I32Parameter::create(parameter_id);
    case DATATYPE_VECTOR2F32:
        return Vector2F32Parameter::create(parameter_id);

    case DATATYPE_VECTOR3I32:
        return Vector3I32Parameter::create(parameter_id);
    case DATATYPE_VECTOR3F32:
        return Vector3F32Parameter::create(parameter_id);

    case DATATYPE_VECTOR4I32:
        return Vector4I32Parameter::create(parameter_id);
    case DATATYPE_VECTOR4F32:
        return Vector4F32Parameter::create(parameter_id);

    case DATATYPE_CUSTOMTYPE:

    default:
        break;
    }

    return nullptr;
}

ParameterPtr ParameterFactory::createParameterReadValue(int16_t parameter_id, datatype_t type_id, std::istream& is)
{
    switch (type_id) {
    // no value
    case DATATYPE_BANG:
        return BangParameter::create(parameter_id);
    case DATATYPE_GROUP:
        return GroupParameter::create(parameter_id);

        // value types
    case DATATYPE_BOOLEAN:
        return readValue(BooleanParameter::create(parameter_id), is);

    case DATATYPE_INT8:
        return readValue(Int8Parameter::create(parameter_id), is);

    case DATATYPE_UINT8:
        return readValue(UInt8Parameter::create(parameter_id), is);

    case DATATYPE_INT16:
        return readValue(Int16Parameter::create(parameter_id), is);

    case DATATYPE_UINT16:
        return readValue(UInt16Parameter::create(parameter_id), is);

    case DATATYPE_INT32:
        return readValue(Int32Parameter::create(parameter_id), is);

    case DATATYPE_UINT32:
        return readValue(UInt32Parameter::create(parameter_id), is);

    case DATATYPE_INT64:
        return readValue(Int64Parameter::create(parameter_id), is);

    case DATATYPE_UINT64:
        return readValue(UInt64Parameter::create(parameter_id), is);

    case DATATYPE_FLOAT32:
        return readValue(Float32Parameter::create(parameter_id), is);

    case DATATYPE_FLOAT64:
        return readValue(Float64Parameter::create(parameter_id), is);

    case DATATYPE_STRING:
        return readValue(StringParameter::create(parameter_id), is);

    case DATATYPE_ENUM:
        return readValue(EnumParameter::create(parameter_id), is);

    case DATATYPE_RGB:
        return readValue(RGBParameter::create(parameter_id), is);
    case DATATYPE_RGBA:
        return readValue(RGBAParameter::create(parameter_id), is);

    case DATATYPE_URI:
        return readValue(URIParameter::create(parameter_id), is);

    case DATATYPE_IPV4:
        return readValue(IPv4Parameter::create(parameter_id), is);

    case DATATYPE_IPV6:
        return readValue(IPv6Parameter::create(parameter_id), is);

    case DATATYPE_RANGE:
        //        {
        //            // get element type
        //            datatype_t element_type_id = static_cast<datatype_t>(is.get());
        //            return createRangeParameterReadValue(parameter_id, element_type_id, is);
        //         }
        return nullptr;

    case DATATYPE_VECTOR2I32:
        return readValue(Vector2I32Parameter::create(parameter_id), is);
    case DATATYPE_VECTOR2F32:
        return readValue(Vector2F32Parameter::create(parameter_id), is);

    case DATATYPE_VECTOR3I32:
        return readValue(Vector3I32Parameter::create(parameter_id), is);
    case DATATYPE_VECTOR3F32:
        return readValue(Vector3F32Parameter::create(parameter_id), is);

    case DATATYPE_VECTOR4I32:
        return readValue(Vector4I32Parameter::create(parameter_id), is);
    case DATATYPE_VECTOR4F32:
        return readValue(Vector4F32Parameter::create(parameter_id), is);

    case DATATYPE_CUSTOMTYPE:
        return nullptr;

    case DATATYPE_ARRAY:
    case DATATYPE_LIST:
    {
        // read element type
        // not implemented
        return nullptr;
    }

    default:
        break;
    }

    return nullptr;
}

ParameterPtr ParameterFactory::createRangeParameter(int16_t parameter_id, datatype_t type_id) {

    switch (type_id) {
    case DATATYPE_INT8:
        return RangeParameter<int8_t>::create(parameter_id);

    case DATATYPE_UINT8:
        return RangeParameter<uint8_t>::create(parameter_id);

    case DATATYPE_INT16:
        return RangeParameter<int16_t>::create(parameter_id);

    case DATATYPE_UINT16:
        return RangeParameter<uint16_t>::create(parameter_id);

    case DATATYPE_INT32:
        return RangeParameter<int32_t>::create(parameter_id);

    case DATATYPE_UINT32:
        return RangeParameter<uint32_t>::create(parameter_id);

    case DATATYPE_INT64:
        return RangeParameter<int64_t>::create(parameter_id);

    case DATATYPE_UINT64:
        return RangeParameter<uint64_t>::create(parameter_id);

    case DATATYPE_FLOAT32:
        return RangeParameter<float>::create(parameter_id);

    case DATATYPE_FLOAT64:
        return RangeParameter<double>::create(parameter_id);

    default:
        // error...?
        break;
    }

    return nullptr;
}

ParameterPtr ParameterFactory::createRangeParameterReadValue(int16_t parameter_id, datatype_t type_id, std::istream& is)
{
    switch (type_id) {
    case DATATYPE_INT8:
        return readValue(RangeParameter<int8_t>::create(parameter_id), is);

    case DATATYPE_UINT8:
        return readValue(RangeParameter<uint8_t>::create(parameter_id), is);

    case DATATYPE_INT16:
        return readValue(RangeParameter<int16_t>::create(parameter_id), is);

    case DATATYPE_UINT16:
        return readValue(RangeParameter<uint16_t>::create(parameter_id), is);

    case DATATYPE_INT32:
        return readValue(RangeParameter<int32_t>::create(parameter_id), is);

    case DATATYPE_UINT32:
        return readValue(RangeParameter<uint32_t>::create(parameter_id), is);

    case DATATYPE_INT64:
        return readValue(RangeParameter<int64_t>::create(parameter_id), is);

    case DATATYPE_UINT64:
        return readValue(RangeParameter<uint64_t>::create(parameter_id), is);

    case DATATYPE_FLOAT32:
        return readValue(RangeParameter<float>::create(parameter_id), is);

    case DATATYPE_FLOAT64:
        return readValue(RangeParameter<double>::create(parameter_id), is);

    default:
        // error...?
        break;
    }

    return nullptr;
}
}
