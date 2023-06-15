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

#ifndef RCP_OPTION_H
#define RCP_OPTION_H

#include "specializetypes.h"

namespace rcp {

    template<typename T>
    class Option {
    public:
        Option()
            : m_hasValue(false)
            , m_changed(false)
        {}

        Option(const T& value)
            : m_value(value)
            , m_hasValue(true)
            , m_changed(false)
        {}

        ~Option()
        {}

        void setValue(const T& value)
        {
            // prevent to clear m_changed if setting the same value more than once
            if (!m_changed)
            {
                m_changed = m_value != value || !m_hasValue;
            }
            m_value = value;
            m_hasValue = true;
        }

        bool hasValue() const
        {
            return m_hasValue;
        }

        bool changed() const
        {
            return m_changed;
        }

        void setUnchanged()
        {
            m_changed = false;
        }

        T& value()
        {
            return m_value;
        }

        const T& value() const
        {
            return m_value;
        }

        void clearValue()
        {
            // prevent to clear m_changed if calling this more than once
            if (!m_changed)
            {
                m_changed = m_hasValue;
            }
            m_hasValue = false;
        }


        Option& operator=(T& value)
        {
            setValue(value);
            return *this;
        }

        const Option& operator=(const T& value)
        {
            setValue(value);
            return *this;
        }

    private:
        T m_value;
        bool m_hasValue;
        bool m_changed;
    };

}

#endif
