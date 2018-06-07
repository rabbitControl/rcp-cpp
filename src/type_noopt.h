#ifndef RCP_TYPEDEFINITION_NOOPT_H
#define RCP_TYPEDEFINITION_NOOPT_H

#include <iostream>

#include "typedefinition.h"
#include "iparameter.h"

namespace rcp {

    template <datatype_t type>
    class TypeDefinitionNoOpt : public ITypeDefinition
    {
    public:
        TypeDefinitionNoOpt() : datatype(type)
        {}

        TypeDefinitionNoOpt(IParameter& param) :
            datatype(type)
        {}

        virtual datatype_t getDatatype() const { return datatype; }

        //------------------------------------
        // implement writeable
        void write(Writer& out, bool all) {

            out.write(static_cast<char>(datatype));

            // terminator
            out.write(static_cast<char>(TERMINATOR));
        }

        //------------------------------------
        // implement optionparser
        void parseOptions(std::istream& is) {
            // no options - expect terminator

            // read one byte
            int did = is.get();

            if (did != TERMINATOR) {
                std::cerr << "error - no-option typedefinition has no terminator!";
            }
        }

        virtual void dump() {
            std::cout << "--- type with no options ---\n";
        }

    private:
        // mandatory
        datatype_t datatype;
    };

    //
    typedef TypeDefinitionNoOpt<DATATYPE_BANG> BangTypeDefinition;
    typedef TypeDefinitionNoOpt<DATATYPE_GROUP> GroupTypeDefinition;

    typedef TypeDefinitionNoOpt<DATATYPE_MAX_> InvalidTypeDefinition;

}

#endif // RCP_TYPEDEFINITION_NOOPT_H
