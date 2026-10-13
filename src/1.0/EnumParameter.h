#ifndef RCP_V10_ENUMPARAMETER_H
#define RCP_V10_ENUMPARAMETER_H

#include "ValueParameter.h"
#include "TypeDefinitionEnum.h"

namespace rcp {
namespace v10 {

class EnumParameter;
using EnumParameterPtr = std::shared_ptr<EnumParameter>;


class EnumParameter
    : public ValueParameter<std::vector<uint32_t>, TypeDefinitionEnum>
{
public:
    static EnumParameterPtr create(uint32_t id)
    {
        return std::make_shared<EnumParameter>(id);
    }

public:
    EnumParameter(uint32_t id)
        : ValueParameter<std::vector<uint32_t>, TypeDefinitionEnum>(id)
        , rcp::Parameter<TypeDefinitionEnum>(id)
        , rcp::IParameter(id)
    {
        if (id > RcpInt::MAX_VALUE)
        {
            throw std::runtime_error("invalid parameter id");
        }
    }

public:
    // any value
    void setStringValue(const std::string& value)
    {
        auto options = m_typedefinition->getAnyOptions();
        auto it = std::find(options.begin(), options.end(), value);

        const size_t index = std::distance(options.begin(), it);

        // TODO: check index against uint32_t?
        if (index < options.size())
        {
            ValueParameter<std::vector<uint32_t>, TypeDefinitionEnum>::setValue(std::vector<uint32_t>{uint32_t(index)});
        }
    }

    // any value
    std::vector<std::string> getStringValues() const
    {
        auto options = m_typedefinition->getAnyOptions();
        std::vector<std::string> result;

        for (uint32_t i : m_value.value())
        {
            if (i < options.size())
            {
                result.push_back(options[i]);
            }
        }

        return result;
    }

};

using EnumParameterPtr = std::shared_ptr<EnumParameter>;

}
}


#endif // ENUMPARAMETER_H
