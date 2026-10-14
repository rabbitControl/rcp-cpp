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

#ifndef RCP_TYPEDEFINITION_H
#define RCP_TYPEDEFINITION_H

#include <cstdint>
#include <istream>

#include "Option.h"
#include "IParameter.h"

namespace rcp
{

static const std::string empty_string = "";

template<uint8_t type>
class TypeDefinition {

public:
    TypeDefinition(IParameter* param)
        : m_parameter(param)
    {
#ifdef DO_MEMORY_TEST
        MemoryTools::countUp();
#endif
    }

#ifdef DO_MEMORY_TEST
    ~TypeDefinition()
    {
        MemoryTools::countDown();
    }
#endif


    uint8_t datatype() const
    {
        return type;
    }

    virtual bool isDirty() const
    {
        return m_dirty;
    }

    // TODO: protect?
    virtual void parseElementOptions(std::istream& is) {}
    virtual void parseOptions(std::istream& is)
    {
        char ch;
        while (is.get(ch))
        {
            // read option prefix
            uint8_t opt = static_cast<uint8_t>(ch & 0x7F);

            // handle v010 terminator
            if (opt == 0)
            {
                break;
            }

            if (!handleOption((opt), is))
            {
                std::cerr << "TypeDefinition: unhandled option: " << std::to_string(opt) << std::endl;

                throw std::runtime_error("TypeDefinition unhandled option: " + std::to_string(opt));
            }

            // handled v10 last option
            if (ch & 0x80)
            {
                break;
            }
        }
    }

    virtual void update(const TypeDefinition& other) {}

    virtual void writeMandatory(Writer& out)
    {
        // TODO: v010 vs v10
        out.write(datatype());
    }
    virtual void write(Writer& out, bool all) = 0;
    virtual void setAllUnchanged()
    {
        m_dirty = false;
    }


protected:
    template<typename T>
    inline void updateOption(Option<T>& o, T value)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        o.setValue(value);
        if (o.changed())
        {
            this->setDirty();
        }
    }


    template<typename T>
    inline void clearOption(Option<T>& o)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        o.clearValue();
        if (o.changed())
        {
            this->setDirty();
        }
    }



    void setDirty()
    {
        if (this->m_parameter)
        {
            this->m_parameter->setDirty();
        }

        m_dirty = true;
    }

    // return:
    // true: handled
    // false: not handled
    virtual bool handleOption(uint8_t optionId, std::istream& is) { return false; }

    IParameter* m_parameter{nullptr};
    bool m_dirty{false};
};

}


#endif // TYPEDEFINITION_H
