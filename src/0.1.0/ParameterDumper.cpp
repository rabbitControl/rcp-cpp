#include "ParameterDumper.h"

#include "GroupParameter.h"
#include "ParameterParser.h"
#include "types.h"
#include "ParameterDefinitions.h"

#include <iostream>
#include <memory>


namespace rcp {
namespace v010 {

void ParameterDumper::dump(IParameterPtr param)
{
    std::cout << "\tdatatype: " << ParameterParser::to_string(datatype_t(param->getDatatype())) << std::endl;
    if (param->hasLabel()) std::cout << "\tlabel: " << param->getLabel() << std::endl;
    if (param->hasDescription()) std::cout << "\tdescription: " << param->getDescription() << std::endl;
    if (param->hasTags()) std::cout << "\tags: " << param->getTags() << std::endl;
    if (param->hasOrder()) std::cout << "\torder: " << param->getOrder() << std::endl;

    if (param->hasParent())
    {
        if (auto p = param->getParent().lock())
        {
            std::cout << "\tparent: " << p->getId() << std::endl;
        }
    }

    if (param->hasUserdata()) std::cout << "\tuser data: " << param->getUserdata().size() << std::endl;
    if (param->hasUserid()) std::cout << "\tuser id: " << param->getUserid() << std::endl;
    if (param->hasReadonly()) std::cout << "\treadonly: " << param->getReadonly() << std::endl;

    if (param->isValueParameter())
    {
        // TODO: how to do this nicer??
        switch (param->getDatatype())
        {
        case DATATYPE_BANG:
        case DATATYPE_GROUP:
            break;


        case DATATYPE_BOOLEAN:
        {
            dumpValue(std::dynamic_pointer_cast<BooleanParameter>(param));
            break;
        }
        case DATATYPE_INT8:
        {
            dumpValue(std::dynamic_pointer_cast<Int8Parameter>(param));
            break;
        }
        case DATATYPE_UINT8:
        {
            dumpValue(std::dynamic_pointer_cast<UInt8Parameter>(param));
            break;
        }
        case DATATYPE_INT16:
        {
            dumpValue(std::dynamic_pointer_cast<Int16Parameter>(param));
            break;
        }
        case DATATYPE_UINT16:
        {
            dumpValue(std::dynamic_pointer_cast<UInt16Parameter>(param));
            break;
        }
        case DATATYPE_INT32:
        {
            dumpValue(std::dynamic_pointer_cast<Int32Parameter>(param));
            break;
        }
        case DATATYPE_UINT32:
        {
            dumpValue(std::dynamic_pointer_cast<UInt32Parameter>(param));
            break;
        }
        case DATATYPE_INT64:
        {
            dumpValue(std::dynamic_pointer_cast<Int64Parameter>(param));
            break;
        }
        case DATATYPE_UINT64:
        {
            dumpValue(std::dynamic_pointer_cast<UInt64Parameter>(param));
            break;
        }
        case DATATYPE_FLOAT32:
        {
            dumpValue(std::dynamic_pointer_cast<Float32Parameter>(param));
            break;
        }
        case DATATYPE_FLOAT64:
        {
            dumpValue(std::dynamic_pointer_cast<Float64Parameter>(param));
            break;
        }

        // TODO: vectors

        case DATATYPE_STRING:
        {
            dumpValue(std::dynamic_pointer_cast<StringParameter>(param));
            break;
        }
        case DATATYPE_ENUM:
        {
            dumpValue(std::dynamic_pointer_cast<EnumParameter>(param));
            break;
        }
        case DATATYPE_RGB:
        {
            dumpValue(std::dynamic_pointer_cast<RGBParameter>(param));
            break;
        }
        case DATATYPE_RGBA:
        {
            dumpValue(std::dynamic_pointer_cast<RGBAParameter>(param));
            break;
        }
        case DATATYPE_URI:
        {
            dumpValue(std::dynamic_pointer_cast<URIParameter>(param));
            break;
        }
        case DATATYPE_IPV4:
        {
            dumpValue(std::dynamic_pointer_cast<IPv4Parameter>(param));
            break;
        }
        case DATATYPE_IPV6:
        {
            dumpValue(std::dynamic_pointer_cast<IPv6Parameter>(param));
            break;
        }
            // TODO: range
        // case DATATYPE_RANGE:
        // {
        //     dumpValue(std::dynamic_pointer_cast<RangeParameter>(param));
        //     break;
        // }

            // TODO: custom
        default:
            std::cerr << "value not dumped for: " << ParameterParser::to_string(datatype_t(param->getDatatype())) << std::endl;
            break;
        }

    }

    std::flush(std::cout);
    std::flush(std::cerr);
}

}
}
