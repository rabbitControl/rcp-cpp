#include "ParameterParser.h"

#include "ParameterFactory.h"
#include "types.h"

#include "CustomParameter.h"
#include "BangParameter.h"
#include "v010StreamTools.h"

namespace rcp {
namespace v010 {


//---------------------------------------------------
// read strings from stream
std::string ParameterParser::readTinyString(std::istream& is)
{
    return readStringFromStream(is, uint8_t{});
}

std::string ParameterParser::readShortString(std::istream& is)
{
    return readStringFromStream(is, uint16_t{});
}

std::string ParameterParser::readLongString(std::istream& is)
{
    return readStringFromStream(is, uint32_t{});
}

std::string ParameterParser::to_string(datatype_t type)
{
    switch (type) {
    case DATATYPE_CUSTOMTYPE: return "DATATYPE_CUSTOMTYPE";
    case DATATYPE_BOOLEAN: return "DATATYPE_BOOLEAN";
    case DATATYPE_INT8: return "DATATYPE_INT8";
    case DATATYPE_UINT8: return "DATATYPE_UINT8";
    case DATATYPE_INT16: return "DATATYPE_INT16";
    case DATATYPE_UINT16: return "DATATYPE_UINT16";
    case DATATYPE_INT32: return "DATATYPE_INT32";
    case DATATYPE_UINT32: return "DATATYPE_UINT32";
    case DATATYPE_INT64: return "DATATYPE_INT64";
    case DATATYPE_UINT64: return "DATATYPE_UINT64";
    case DATATYPE_FLOAT32: return "DATATYPE_FLOAT32";
    case DATATYPE_FLOAT64: return "DATATYPE_FLOAT64";
    case DATATYPE_VECTOR2I32: return "DATATYPE_VECTOR2I32";
    case DATATYPE_VECTOR2F32: return "DATATYPE_VECTOR2F32";
    case DATATYPE_VECTOR3I32: return "DATATYPE_VECTOR3I32";
    case DATATYPE_VECTOR3F32: return "DATATYPE_VECTOR3F32";
    case DATATYPE_VECTOR4I32: return "DATATYPE_VECTOR4I32";
    case DATATYPE_VECTOR4F32: return "DATATYPE_VECTOR4F32";
    case DATATYPE_STRING: return "DATATYPE_STRING";
    case DATATYPE_RGB: return "DATATYPE_RGB";
    case DATATYPE_RGBA: return "DATATYPE_RGBA";
    case DATATYPE_ENUM: return "DATATYPE_ENUM";
    case DATATYPE_ARRAY: return "DATATYPE_ARRAY";
    case DATATYPE_LIST: return "DATATYPE_LIST";
    case DATATYPE_BANG: return "DATATYPE_BANG";
    case DATATYPE_GROUP: return "DATATYPE_GROUP";
    case DATATYPE_URI: return "DATATYPE_URI";
    case DATATYPE_IPV4: return "DATATYPE_IPV4";
    case DATATYPE_IPV6: return "DATATYPE_IPV6";
    case DATATYPE_RANGE: return "DATATYPE_RANGE";
    case DATATYPE_IMAGE: return "DATATYPE_IMAGE";
    case DATATYPE_MAX_: return "DATATYPE_MAX_";
        break;
    }

    return "unknown type: " + std::to_string(type);
}



IParameterPtr
ParameterParser::parse(std::istream& is, ParameterManagerPtr manager)
{
    // get id and type
    int16_t parameter_id = 0;
    parameter_id = readFromStream(is, parameter_id);

    if (is.eof())
    {
        return nullptr;
    }

    // get parameter type_id
    datatype_t type_id = static_cast<datatype_t>(is.get());

    // validity check
    if (is.eof() ||
        type_id < 1 ||
        type_id >= DATATYPE_MAX_)
    {
        // fail
        return nullptr;
    }

    //
    IParameterPtr param;

    // handle certain datatypes
    if (type_id == DATATYPE_RANGE)
    {
        // get element type
        datatype_t element_type_id = static_cast<datatype_t>(is.get());

        param = ParameterFactory::createRangeParameter(parameter_id, element_type_id);
        if (!param)
        {
            std::cerr << "could not create rangeparameter with element_type: " << element_type_id << "\n";
            return nullptr;
        }

        param->parseElementTypeOptions(is);
    }
    else if (type_id == DATATYPE_ARRAY)
    {
        // TODO
    }
    else if (type_id == DATATYPE_LIST)
    {
        // TODO
    }
    else if (type_id == DATATYPE_CUSTOMTYPE)
    {
        // read size from stream
        uint32_t size = readFromStream(is, size);

        param = CustomParameter::create(parameter_id, size);

        if (!param)
        {
            std::cerr << "could not create custom parameter\n";
            return nullptr;
        }

        // param->getTypeDefinition().parseOptions(is);
    }
    else
    {
        param = ParameterFactory::createParameter(parameter_id, type_id);

        if (!param) {
            return nullptr;
        }

        // param->getTypeDefinition().parseOptions(is);
    }

    param->setManager(manager);
    parseParameterOptions(param, is, manager);

    return param;
}


void ParameterParser::parseParameterOptions(IParameterPtr param, std::istream& is, ParameterManagerPtr manager)
{
    param->parseTypeOptions(is);

    // parameter options
    char ch;
    while (is.get(ch))
    {
        // read option prefix
        parameter_options_t opt = static_cast<parameter_options_t>(ch);

        // check stream
        CHECK_STREAM;

        if (opt == TERMINATOR)
        {
            break;
        }


        switch (opt)
        {
        case PARAMETER_OPTIONS_LABEL: {

            // peek one
            int peek = is.peek();
            CHECK_STREAM;

            char code[4];
            code[3] = 0;
            while (peek > 0)
            {
                is.read(code, 3);

                CHECK_STREAM;

                std::string code_str(code);
                std::string label = readTinyString(is);

                CHECK_STREAM;

                if (code_str == language_string_any)
                {
                    param->m_hasLabel = true;
                    param->label = label;
                }
                else
                {
                    param->languageLabel[code_str] = label;
                }

                // peek next byte
                peek = is.peek();
            }

            CHECK_STREAM;

            // read 0-byte off the stream
            is.get();
            break;
        }
        case PARAMETER_OPTIONS_DESCRIPTION: {

            // peek one
            int peek = is.peek();
            CHECK_STREAM;

            char code[4];
            code[3] = 0;
            while (peek > 0)
            {
                is.read(code, 3);

                CHECK_STREAM;

                std::string code_str(code);
                std::string description = readShortString(is);

                CHECK_STREAM;

                if (code_str == language_string_any)
                {
                    param->m_hasDescription = true;
                    param->description = description;
                }
                else
                {
                    param->languageDescription[code_str] = description;
                }

                // peek next byte
                peek = is.peek();
            }

            CHECK_STREAM;

            // read 0-byte off the stream
            is.get();
            break;
        }
        case PARAMETER_OPTIONS_TAGS: {
            std::string st = readTinyString(is);
            CHECK_STREAM;

            param->tags = st;
            break;
        }
        case PARAMETER_OPTIONS_ORDER: {

            int32_t val = readFromStream(is, val);
            CHECK_STREAM;

            param->order = val;
            break;
        }
        case PARAMETER_OPTIONS_PARENTID: {

            int16_t parent_id = readFromStream(is, parent_id);

            CHECK_STREAM;

            if (manager)
            {
                if (parent_id != 0)
                {
                    auto param_parent = manager->getParameter(parent_id);
                    if (param_parent)
                    {
                        // NOTE:
                        // parsed parameter is is a proxy parameter
                        // _do_not_ set this paremter as a child of the parent!
                        // a proxy-parameter is either updating an exisitng parameter in cache
                        // or is added. if the proxy-parameter is added it gets added to the parent too

                        if (param_parent->isGroup())
                        {
                            // this is a proxy-parameter
                            // this means that "parent" is not set - it's a new parameter
                            // skip this

                            // if (auto p = parent.lock())
                            // {
                            //     if (param_parent->getId() != p->getId())
                            //     {
                            //         // remove parameter-id from parent...
                            //         p->removeChild(param);
                            //     }
                            // }

                            param->m_parent = std::dynamic_pointer_cast<IGroupParameter>(param_parent);
                        }
                        else
                        {
                            // parent not a group!
                        }
                    }
                    else
                    {
                        // put it into list, look parent up later
                        manager->addMissingParent(parent_id, param);
                    }
                }
                else
                {
                    // set root group as parent
                    param->m_parent = manager->getRootGroup();
                }
            }
            else
            {
                // no manager!
            }


            break;
        }

        case PARAMETER_OPTIONS_WIDGET:
            // TODO: handle this
            break;

        case PARAMETER_OPTIONS_USERDATA:
        {
            uint32_t size = readFromStream(is, size);
            param->m_userdata.resize(size);
            is.read(param->m_userdata.data(), size);
            break;
        }
        case PARAMETER_OPTIONS_USERID: {

            std::string st = readTinyString(is);
            CHECK_STREAM;

            param->m_userid = st;
            break;
        }

        case PARAMETER_OPTIONS_READONLY: {

            bool readonly = readFromStream(is, readonly);
            CHECK_STREAM;

            param->readonly = readonly;
            break;
        }

        default:
            // handle all other options in "handleOption"
            if (!param->handleOption(opt, is))
            {
                // error!
                return;
            }
            break;
        }

    } // while
}

IParameterPtr ParameterParser::parseUpdateValue(std::istream &is)
{
    // read id
    int16_t parameter_id = 0;
    parameter_id = readFromStream(is, parameter_id);

    // get parameter type_id
    datatype_t type_id = static_cast<datatype_t>(is.get());

    if (type_id == DATATYPE_GROUP)
    {
        return nullptr;
    }

    if (type_id == DATATYPE_BANG)
    {
        BangParameterPtr param = BangParameter::create(parameter_id);
        if (!param)
        {
            std::cerr << "[ParameterParser][0.1.0] could not create bang parameter\n";
        }

        return param;
    }

    if (type_id == DATATYPE_CUSTOMTYPE)
    {
        // read size from stream
        uint32_t size = readFromStream(is, size);

        CustomParameterPtr param = CustomParameter::create(parameter_id, size);
        if (!param)
        {
            std::cerr << "[ParameterParser][0.1.0] could not create custom parameter\n";
            return nullptr;
        }

        return ParameterFactory::readValue(param, is);
    }

    auto param = ParameterFactory::createParameterReadValue(parameter_id, type_id, is);

    if (!param)
    {
        std::cerr << "[ParameterParser][0.1.0] could not create parameter\n";
    }

    return param;
}

}
}
