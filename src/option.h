#ifndef RCP_OPTION_H
#define RCP_OPTION_H

#include "specializetypes.h"

namespace rcp {

    template<typename T>
    class Option {
    public:
        Option() :
            m_hasValue(false)
        {}

        Option(const T& value) :
            m_value(value)
          , m_hasValue(true)
        {}

        ~Option()
        {}

        void setValue(const T& value) {
            m_value = value;
            m_hasValue = true;
        }

        bool hasValue() const {
            return m_hasValue;
        }

        T& getValue() {
            return m_value;
        }

        void clearValue() const {
            m_hasValue = false;
        }


        Option& operator=(T& value)
        {
            m_value = value;
            m_hasValue = true;
            return *this;
        }

        const Option& operator=(const T& value)
        {
            m_value = value;
            m_hasValue = true;
            return *this;
        }

    private:
        T m_value;
        bool m_hasValue;
    };

}

#endif
