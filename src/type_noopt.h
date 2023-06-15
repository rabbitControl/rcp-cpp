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

        TypeDefinitionNoOpt(IParameter& /*param*/) :
            datatype(type)
        {}

        //------------------------------------
        // Writeable
        void write(Writer& out, bool /*all*/) override
        {
            writeMandatory(out);

            // terminator
            out.write(static_cast<char>(TERMINATOR));
        }

        //------------------------------------
        // ITypeDefinition
        datatype_t getDatatype() const override
        {
            return datatype;
        }

        void writeMandatory(Writer& out) const override
        {
            out.write(static_cast<char>(datatype));
        }

        bool anyOptionChanged() const override
        {
            return false;
        }

        void dump() override
        {
            std::cout << "--- type with no options ---\n";
        }

        //------------------------------------
        // IOptionparser
        void parseOptions(std::istream& is) override
        {
            // no options - expect terminator

            // read one byte
            int did = is.get();

            if (did != TERMINATOR) {
                std::cerr << "error - no-option typedefinition has no terminator!";
            }
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
