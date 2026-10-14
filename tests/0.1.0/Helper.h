#ifndef RCP_TESTS_V10_HELPER_H
#define RCP_TESTS_V10_HELPER_H

#include <rcp/ParameterManager.h>
#include <rcp/StringStreamWriter.h>
#include <rcp/0.1.0/ParameterWriter.h>
#include <rcp/0.1.0/ParameterParser.h>

namespace rcp {
namespace v010 {


static inline void write(IParameterPtr param, bool all)
{
    StringStreamWriter writer;
    ParameterWriter pw;

    pw.write(param, writer, all);

    writer.dump();
}

template<typename T>
std::shared_ptr<T> reparse(IParameterPtr param, rcp::IParameterPtr group = nullptr)
{
    StringStreamWriter writer;
    ParameterWriter pw;

    pw.write(param, writer, false);

    auto manager = ParameterManager::create();
    manager->_addParameter(group);

    auto parsed_param = ParameterParser::parse(writer.getBuffer(), manager);

    if (parsed_param)
    {
        return std::dynamic_pointer_cast<T>(parsed_param);
    }

    return nullptr;
}


}
}
#endif // HELPER_H
