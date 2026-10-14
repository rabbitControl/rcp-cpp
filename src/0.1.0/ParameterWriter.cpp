#include "rcp/0.1.0/ParameterWriter.h"

#include "rcp/IParameter.h"

#include "rcp/0.1.0/types.h"

namespace rcp {
namespace v010 {



void ParameterWriter::writeTinyString(Writer& out, const std::string& s)
{
    if (s.length() >= UINT8_MAX)
    {
        out.write(UINT8_MAX);
        out.write(s.substr(0, UINT8_MAX), false); // write without length-prefix
    }
    else
    {
        out.write(static_cast<uint8_t>(s.length()));
        out.write(s, false); // write without length-prefix
    }
}

void ParameterWriter::writeShortString(Writer& out, const std::string& s)
{
    if (s.length() >= UINT16_MAX)
    {
        out.write(UINT16_MAX);
        out.write(s.substr(0, UINT16_MAX), false); // write without length-prefix
    }
    else
    {
        out.write(static_cast<uint16_t>(s.length()));
        out.write(s, false); // write without length-prefix
    }
}

void ParameterWriter::writeString(Writer& out, const std::string& s)
{
    if (s.length() >= UINT32_MAX)
    {
        out.write(UINT32_MAX);
        out.write(s.substr(0, UINT32_MAX), false); // write without length-prefix
    }
    else
    {
        out.write(static_cast<uint32_t>(s.length()));
        out.write(s, false); // write without length-prefix
    }
}



// write label and all language labels
void ParameterWriter::writeLabel(IParameterPtr parameter, Writer& out)
{
    out.write(static_cast<char>(PARAMETER_OPTIONS_LABEL));

    if (parameter->m_hasLabel)
    {
        out.write(language_string_any, false);
        writeTinyString(out, parameter->label);
    }

    for (auto& l : parameter->languageLabel)
    {
        if (l.first.length() < 3)
        {
            continue;
        }

        out.write(l.first.substr(0, 3), false);
        writeTinyString(out, l.second);
    }

    out.write(static_cast<char>(TERMINATOR));
}

// write description and language description
void ParameterWriter::writeDescription(IParameterPtr parameter, Writer& out)
{
    out.write(static_cast<char>(PARAMETER_OPTIONS_DESCRIPTION));

    if (parameter->m_hasDescription)
    {
        out.write(language_string_any, false);
        writeShortString(out, parameter->description);
    }

    for (auto& l : parameter->languageDescription)
    {
        if (l.first.length() < 3)
        {
            continue;
        }

        out.write(l.first.substr(0, 3), false);
        writeShortString(out, l.second);
    }

    out.write(static_cast<char>(TERMINATOR));
}


void ParameterWriter::write(IParameterPtr parameter, Writer &out, bool all)
{
    if (!all &&
        parameter->onlyValueChanged())
    {
        writeUpdateValue(parameter, out);
        return;
    }


    // write parameter

    out.write(int16_t(parameter->getId()));

    parameter->writeTypeDefinition(out, all);

    parameter->writeValueOption(out, all, PARAMETER_OPTIONS_VALUE);


    // label
    if (parameter->m_hasLabel || parameter->languageLabel.size() > 0)
    {
        if (all || parameter->labelChanged)
        {
            writeLabel(parameter, out);

            if (!all)
            {
                parameter->labelChanged = false;
            }
        }
    }
    else if(parameter->labelChanged)
    {
        out.write(static_cast<char>(PARAMETER_OPTIONS_LABEL));
        out.write(static_cast<char>(0));
        parameter->labelChanged = false;
    }


    // description
    if (parameter->m_hasDescription || parameter->languageDescription.size() > 0)
    {
        if (all || parameter->descriptionChanged)
        {
            writeDescription(parameter, out);

            if (!all)
            {
                parameter->descriptionChanged = false;
            }
        }
    }
    else if (parameter->descriptionChanged)
    {
        out.write(static_cast<char>(PARAMETER_OPTIONS_DESCRIPTION));
        out.write(static_cast<char>(0));
        parameter->descriptionChanged = false;
    }


    // tags
    if (parameter->tags.hasValue())
    {
        if (all || parameter->tags.changed())
        {
            out.write(static_cast<char>(PARAMETER_OPTIONS_TAGS));
            writeTinyString(out, parameter->tags.value());

            if (!all)
            {
                parameter->tags.setUnchanged();
            }
        }
    }
    else if (parameter->tags.changed())
    {
        out.write(static_cast<char>(PARAMETER_OPTIONS_TAGS));
        writeTinyString(out, "");
        parameter->tags.setUnchanged();
    }

    // order
    if (parameter->order.hasValue())
    {
        if (all || parameter->order.changed())
        {
            out.write(static_cast<char>(PARAMETER_OPTIONS_ORDER));
            out.write(parameter->order.value());

            if (!all)
            {
                parameter->order.setUnchanged();
            }
        }
    }
    else if (parameter->order.changed())
    {
        out.write(static_cast<char>(PARAMETER_OPTIONS_ORDER));
        out.write(static_cast<int32_t>(0));
        parameter->order.setUnchanged();
    }


    // parent id
    if (auto p = parameter->m_parent.lock())
    {
        if (all || parameter->parentChanged)
        {
            out.write(static_cast<char>(PARAMETER_OPTIONS_PARENTID));

            // cast to IParameter* due to incomplete type
            out.write(((IParameter*)p.get())->getId());

            if (!all)
            {
                parameter->parentChanged = false;
            }
        }        
    }
    else if (parameter->parentChanged)
    {
        out.write(static_cast<char>(PARAMETER_OPTIONS_PARENTID));
        out.write(static_cast<int16_t>(0));
        parameter->parentChanged = false;
    }


    // TODO: widget

    // userdata
    if (!parameter->m_userdata.empty())
    {
        if (all || parameter->userdataChanged)
        {
            out.write(static_cast<char>(PARAMETER_OPTIONS_USERDATA));
            out.write(static_cast<uint32_t>(parameter->m_userdata.size()));
            out.write(parameter->m_userdata.data(), parameter->m_userdata.size());

            if (!all)
            {
                parameter->userdataChanged = false;
            }
        }
    }
    else if (parameter->userdataChanged)
    {
        out.write(static_cast<char>(PARAMETER_OPTIONS_USERDATA));
        out.write(static_cast<uint32_t>(0));
        parameter->userdataChanged = false;
    }


    // userid
    if (parameter->m_userid.hasValue())
    {
        if (all || parameter->m_userid.changed())
        {
            out.write(static_cast<char>(PARAMETER_OPTIONS_USERID));
            writeTinyString(out, parameter->m_userid.value());

            if (!all)
            {
                parameter->m_userid.setUnchanged();
            }
        }
    }
    else if (parameter->m_userid.changed())
    {
        out.write(static_cast<char>(PARAMETER_OPTIONS_USERID));
        writeTinyString(out, "");
        parameter->m_userid.setUnchanged();
    }


    // readonly
    if (parameter->readonly.hasValue())
    {
        if (all || parameter->readonly.changed())
        {
            out.write(static_cast<char>(PARAMETER_OPTIONS_READONLY));
            out.write(parameter->readonly.value());

            if (!all)
            {
                parameter->readonly.setUnchanged();
            }
        }
    }
    else if (parameter->readonly.changed())
    {
        out.write(static_cast<char>(PARAMETER_OPTIONS_READONLY));
        out.write(false);
        parameter->readonly.setUnchanged();
    }


    // write any other options?
    // parameter->writeOtherOptions(out, all);



    // terminator
    out.write(static_cast<char>(TERMINATOR));
}

void ParameterWriter::writeUpdateValue(IParameterPtr parameter, Writer &out)
{
    //
    out.write(int16_t(parameter->getId()));

    // write mandatory datatype
    parameter->writeTypeMandatory(out);

    parameter->writeValue(out);
}

}
}
