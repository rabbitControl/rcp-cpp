#ifndef RCP_OPTIONPARSER_H
#define RCP_OPTIONPARSER_H

#include <istream>

namespace rcp {

    class IOptionparser {
    public:
        virtual void parseOptions(std::istream& is) = 0;
    };
}

#endif // RCP_OPTIONPARSER_H
