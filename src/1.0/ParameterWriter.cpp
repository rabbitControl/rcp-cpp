#include "rcp/1.0/ParameterWriter.h"

#include "rcp/1.0/IParameter.h"

#include "rcp/1.0/GroupParameter.h"
#include "rcp/1.0/RcpInt.h"
#include "rcp/1.0/RcpLanguageString.h"
#include "rcp/1.0/types.h"

namespace rcp {
namespace v10 {


inline void ParameterWriter::writeLabel(IParameterPtr parameter, Writer& out)
{
    if (parameter->m_hasLabel)
    {
        out.write(language_string_any, false);
        writeString(out, parameter->label);
    }

    for (auto& l : parameter->languageLabel)
    {
        if (l.first.length() < 3)
        {
            continue;
        }

        out.write(l.first.substr(0, 3), false);
        writeString(out, l.second);
    }

    out.write(static_cast<char>(0));
}


inline void ParameterWriter::writeDescription(IParameterPtr parameter, Writer& out)
{
    if (parameter->m_hasDescription)
    {
        out.write(language_string_any, false);
        writeString(out, parameter->description);
    }

    for (auto& s : parameter->languageDescription)
    {
        if (s.first.length() < 3)
        {
            continue;
        }

        out.write(s.first.substr(0, 3), false);
        writeString(out, s.second);
    }

    out.write(static_cast<char>(0));
}


void ParameterWriter::write(IParameterPtr parameter, Writer &out, bool all)
{

    if (!parameter)
    {
        return;
    }

    if (!all &&
        parameter->onlyValueChanged())
    {
        writeUpdateValue(parameter, out);
        return;
    }

    RcpInt(parameter->getId()).write(out, true);

    parameter->writeTypeDefinition(out, all);


    // for (auto id : parameter->m_changed)
    // {
    //     std::cout << "CHANGED: " << std::to_string(id) << std::endl;
    // }

    if (all)
    {        
        if (parameter->hasValueOption()) parameter->m_changed.insert(PARAMETER_OPTIONS_VALUE);
        if (parameter->m_hasLabel) parameter->m_changed.insert(PARAMETER_OPTIONS_LABEL);
        if (parameter->m_hasDescription) parameter->m_changed.insert(PARAMETER_OPTIONS_DESCRIPTION);
        if (parameter->tags.hasValue()) parameter->m_changed.insert(PARAMETER_OPTIONS_TAGS);
        if (parameter->order.hasValue()) parameter->m_changed.insert(PARAMETER_OPTIONS_ORDER);
        if (parameter->hasParent()) parameter->m_changed.insert(PARAMETER_OPTIONS_PARENTID);
        // if (parameter->hasWidget()) parameter->m_changed.insert(PARAMETER_OPTIONS_WIDGET);
        if (parameter->hasUserdata()) parameter->m_changed.insert(PARAMETER_OPTIONS_USERDATA);
        if (parameter->m_userid.hasValue()) parameter->m_changed.insert(PARAMETER_OPTIONS_USERID);
        if (parameter->readonly.hasValue()) parameter->m_changed.insert(PARAMETER_OPTIONS_READONLY);
        if (parameter->enabled.hasValue()) parameter->m_changed.insert(PARAMETER_OPTIONS_ENABLED);
    }


    // write options
    while (!parameter->m_changed.empty())
    {
        uint8_t masked_id = *parameter->m_changed.begin() | (parameter->m_changed.size() == 1 ? Terminator : 0x00);

        // TODO: id validity-check?
        // *parameter->m_changed.begin()

        // std::cout << "CHANGED: " << std::hex << std::to_string(*parameter->m_changed.begin()) << std::endl;

        switch (*parameter->m_changed.begin())
        {
        case PARAMETER_OPTIONS_VALUE:

            parameter->writeValueOption(out, all, masked_id);

            break;

        case PARAMETER_OPTIONS_LABEL:

            if (parameter->m_hasLabel || parameter->languageLabel.size() > 0)
            {
                if (all || parameter->labelChanged)
                {
                    out.write(static_cast<char>(masked_id));

                    // writeLabel(parameter, out);
                    RcpLanguageString(parameter->label, parameter->languageLabel).write(out, true);

                    if (!all)
                    {
                        parameter->labelChanged = false;
                    }
                }
            }
            else if (parameter->labelChanged)
            {
                out.write(static_cast<char>(masked_id));
                out.write(static_cast<char>(0));
                parameter->labelChanged = false;
            }

            break;

        case PARAMETER_OPTIONS_DESCRIPTION:

            if (parameter->m_hasDescription || parameter->languageDescription.size() > 0)
            {
                if (all || parameter->descriptionChanged)
                {
                    out.write(static_cast<char>(masked_id));

                    // writeDescription(parameter, out);
                    RcpLanguageString(parameter->description, parameter->languageDescription).write(out, true);

                    if (!all)
                    {
                        parameter->descriptionChanged = false;
                    }
                }
            }
            else if (parameter->descriptionChanged)
            {
                out.write(static_cast<char>(masked_id));
                out.write(static_cast<char>(0));
                parameter->descriptionChanged = false;
            }

            break;

        case PARAMETER_OPTIONS_TAGS:

            if (parameter->tags.hasValue())
            {
                if (all || parameter->tags.changed())
                {
                    out.write(static_cast<char>(masked_id));
                    writeString(out, parameter->tags.value());

                    if (!all)
                    {
                        parameter->tags.setUnchanged();
                    }
                }
            }
            else if (parameter->tags.changed())
            {
                out.write(static_cast<char>(masked_id));
                writeString(out, "");
                parameter->tags.setUnchanged();
            }


            break;


        case PARAMETER_OPTIONS_ORDER:

            if (parameter->order.hasValue())
            {
                if (all || parameter->order.changed())
                {
                    out.write(static_cast<char>(masked_id));

                    RcpInt(parameter->order.value()).write(out, true);

                    if (!all)
                    {
                        parameter->order.setUnchanged();
                    }
                }
            }
            else if (parameter->order.changed())
            {
                out.write(static_cast<char>(masked_id));
                out.write(static_cast<char>(Terminator)); // RcpInt(0)
                parameter->order.setUnchanged();
            }

            break;

        case PARAMETER_OPTIONS_PARENTID:

            if (auto p = parameter->m_parent.lock())
            {
                auto parent_group = std::dynamic_pointer_cast<GroupParameter>(p);

                if (parent_group)
                {
                    if (all || parameter->parentChanged)
                    {
                        out.write(static_cast<char>(masked_id));

                        RcpInt(parent_group->getId()).write(out, true);

                        if (!all)
                        {
                            parameter->parentChanged = false;
                        }
                    }
                }
            }
            else if (parameter->parentChanged)
            {
                out.write(static_cast<char>(masked_id));
                out.write(static_cast<char>(Terminator)); // RcpInt(0)
                parameter->parentChanged = false;
            }

            break;


        case PARAMETER_OPTIONS_WIDGET:
            // TODO
            throw std::runtime_error("widget not implemented!");
            break;


        case PARAMETER_OPTIONS_USERDATA:

            if (!parameter->m_userdata.empty())
            {
                if (all || parameter->userdataChanged)
                {
                    out.write(static_cast<char>(masked_id));
                    RcpInt(parameter->m_userdata.size()).write(out, true);
                    out.write(parameter->m_userdata.data(), parameter->m_userdata.size());

                    if (!all)
                    {
                        parameter->userdataChanged = false;
                    }
                }
            }
            else if (parameter->userdataChanged)
            {
                out.write(static_cast<char>(masked_id));
                out.write(static_cast<char>(Terminator)); // RcpInt(0)
                parameter->userdataChanged = false;
            }

            break;

        case PARAMETER_OPTIONS_USERID:

            if (parameter->m_userid.hasValue())
            {
                if (all || parameter->m_userid.changed())
                {
                    out.write(static_cast<char>(masked_id));
                    writeString(out, parameter->m_userid.value());

                    if (!all)
                    {
                        parameter->m_userid.setUnchanged();
                    }
                }
            }
            else if (parameter->m_userid.changed())
            {
                out.write(static_cast<char>(masked_id));
                writeString(out, empty_string);
                parameter->m_userid.setUnchanged();
            }

            break;

        case PARAMETER_OPTIONS_READONLY:

            if (parameter->readonly.hasValue())
            {
                if (all || parameter->readonly.changed())
                {
                    out.write(static_cast<char>(masked_id));
                    out.write(parameter->readonly.value());

                    if (!all)
                    {
                        parameter->readonly.setUnchanged();
                    }
                }
            }
            else if (parameter->readonly.changed())
            {
                out.write(static_cast<char>(masked_id));
                out.write(false);
                parameter->readonly.setUnchanged();
            }

            break;

        case PARAMETER_OPTIONS_ENABLED:

            if (parameter->enabled.hasValue())
            {
                if (all || parameter->enabled.changed())
                {
                    out.write(static_cast<char>(masked_id));
                    out.write(parameter->enabled.value());

                    if (!all)
                    {
                        parameter->enabled.setUnchanged();
                    }
                }
            }
            else if (parameter->enabled.changed())
            {
                out.write(static_cast<char>(masked_id));
                out.write(true);
                parameter->enabled.setUnchanged();
            }

            break;

        default:
            std::cout << "unhandled option: " << std::to_string(masked_id) << std::endl;
            break;
        }

        //
        parameter->m_changed.erase(parameter->m_changed.begin());
    }

    parameter->setAllUnchanged();
}

void ParameterWriter::writeUpdateValue(IParameterPtr parameter, Writer &out)
{
    RcpInt(parameter->getId()).write(out, true);

    // write mandatory datatype
    parameter->writeTypeMandatory(out);

    parameter->writeValue(out);
}

void ParameterWriter::writeString(Writer &out, const std::string &str)
{
    RcpInt(str.length()).write(out, true);
    out.write(str, false); // no size prefix
}


}
}
