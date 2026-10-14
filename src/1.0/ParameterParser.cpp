#include "rcp/1.0/ParameterParser.h"

#include "rcp/ParameterManager.h"
#include "rcp/StreamTools.h"

#include "rcp/1.0/BangParameter.h"
#include "rcp/1.0/CustomParameter.h"
#include "rcp/1.0/ParameterFactory.h"
#include "rcp/1.0/RcpInt.h"
#include "rcp/1.0/RcpLanguageString.h"
#include "rcp/1.0/RcpString.h"
#include "rcp/1.0/types.h"

namespace rcp {
namespace v10 {


std::string ParameterParser::readString(std::istream &is)
{
    return RcpString::parse(is);
}

IParameterPtr ParameterParser::parse(std::istream &is, ParameterManagerPtr manager)
{
    auto parameter_id = RcpInt::parse(is).value(); // throws

    char masked_type_id = is.get();

    bool has_type_options = !(masked_type_id & Terminator);

    datatype_t type_id = static_cast<datatype_t>(masked_type_id & TerminatorMask);

    // validity check
    if (type_id < 1 ||
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

        char masked_element_type_id = is.get();

        bool has_element_type_options = !(masked_element_type_id & Terminator);

        datatype_t element_type_id = static_cast<datatype_t>(masked_element_type_id & TerminatorMask);


        param = ParameterFactory::createRangeParameter(parameter_id, element_type_id);
        if (!param)
        {
            std::cerr << "could not create rangeparameter with element_type: " << element_type_id << "\n";
            return nullptr;
        }

        // parse element options
        if (has_element_type_options)
        {
            param->parseElementTypeOptions(is);
        }
    }
    else if (type_id == DATATYPE_ARRAY)
    {
        // TODO
    }
    else if (type_id == DATATYPE_CUSTOMTYPE)
    {
        //read size from stream
        uint32_t size = RcpInt::parse(is).value();

        param = CustomParameter::create(parameter_id, size);

        if (!param)
        {
            std::cerr << "could not create custom parameter\n";
            return nullptr;
        }
    }
    else
    {
        param = ParameterFactory::createParameter(parameter_id, type_id);

        if (!param)
        {
            return nullptr;
        }
    }


    param->setManager(manager);

    if (has_type_options)
    {
        param->parseTypeOptions(is);
    }

    parseParameterOptions(param, is, manager);

    return param;
}

void ParameterParser::parseParameterOptions(IParameterPtr param, std::istream &is, ParameterManagerPtr manager)
{    
    // parameter options
    char masked_opt;
    while (is.get(masked_opt))
    {
        // read option prefix
        bool last_option = masked_opt & Terminator;

        parameter_options_t opt = static_cast<parameter_options_t>(masked_opt & ~Terminator);


        switch (opt)
        {
        case PARAMETER_OPTIONS_LABEL:
        {
            auto lng = RcpLanguageString::parse(is);

            if (lng.hasAnyString())
            {
                param->label = lng.anyString();
                param->m_hasLabel = true;
            }

            param->languageLabel = lng.languageMap();

            break;
        }
        case PARAMETER_OPTIONS_DESCRIPTION:
        {
            auto lng = RcpLanguageString::parse(is);

            if (lng.hasAnyString())
            {
                param->description = lng.anyString();
                param->m_hasDescription = true;
            }

            param->languageDescription = lng.languageMap();

            break;
        }
        case PARAMETER_OPTIONS_TAGS: {
            std::string st = readString(is);
            CHECK_STREAM;

            param->tags = st;
            break;
        }
        case PARAMETER_OPTIONS_ORDER: {

            int32_t val = RcpInt::parse(is).value();
            CHECK_STREAM;

            param->order = val;
            break;
        }
        case PARAMETER_OPTIONS_PARENTID: {

            uint32_t parent_id = RcpInt::parse(is).value();

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
            throw std::runtime_error("widget not implemented!");
            break;

        case PARAMETER_OPTIONS_USERDATA:
        {
            uint32_t size = RcpInt::parse(is).value();
            param->m_userdata.resize(size);
            is.read(param->m_userdata.data(), size);
            break;
        }

        case PARAMETER_OPTIONS_USERID:
        {
            std::string st = readString(is);
            CHECK_STREAM;

            param->m_userid = st;
            break;
        }

        case PARAMETER_OPTIONS_READONLY:
        {
            bool r = readFromStream(is, r);
            CHECK_STREAM;

            param->readonly = r;
            break;
        }

        case PARAMETER_OPTIONS_ENABLED:
        {
            bool e = readFromStream(is, e);
            CHECK_STREAM;

            param->enabled = e;
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

        if (last_option)
        {
            break;
        }

    } // while
}

IParameterPtr ParameterParser::parseUpdateValue(std::istream &is)
{
    // read id
    IdType parameter_id = RcpInt::parse(is).value();

    // get parameter type_id
    datatype_t type_id = static_cast<datatype_t>(is.get());

    if (type_id == DATATYPE_GROUP)
    {
        return nullptr;
    }

    if (type_id == DATATYPE_BANG)
    {
        auto param = BangParameter::create(parameter_id);
        if (!param)
        {
            std::cerr << "[ParameterParser][1.0] could not create bang parameter\n";
        }

        return param;
    }

    if (type_id == DATATYPE_CUSTOMTYPE)
    {
        // read size from stream
        uint32_t size = RcpInt::parse(is).value();

        CustomParameterPtr param = std::make_shared<CustomParameter>(parameter_id, size);
        if (!param)
        {
            std::cerr << "[ParameterParser][1.0] could not create custom parameter\n";
            return nullptr;
        }

        return ParameterFactory::readValue(param, is);
    }

    auto param = ParameterFactory::createParameterReadValue(parameter_id, type_id, is);
    if (!param)
    {
        std::cerr << "[ParameterParser][1.0] could not create parameter\n";
    }

    return param;
}


}
}


