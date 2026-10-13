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

#ifndef RCP_IPARAMETER_H
#define RCP_IPARAMETER_H

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <functional>
#include <mutex>

#include "0.1.0/ParameterParser.h"
#include "0.1.0/ParameterWriter.h"

#include "1.0/ParameterParser.h"
#include "1.0/ParameterWriter.h"

#include "MemoryTools.h"
#include "Option.h"
#include "Writer.h"

namespace rcp {

using IdType = int32_t; // internal parameter it type

class IGroupParameter;
using IGroupParameterPtr = std::shared_ptr<IGroupParameter>;


class IParameter;
using IParameterPtr = std::shared_ptr<IParameter>;

class ParameterManager;
using ParameterManagerPtr = std::shared_ptr<ParameterManager>;


class IParameter
    // : public Writeable
{
public:
    virtual IdType getId() const = 0;
    virtual uint8_t getDatatype() const = 0;

    //--------------------------------
    // optional
    virtual std::string getLabel() const = 0;
    virtual void setLabel(const std::string& label) = 0;
    virtual bool hasLabel() const = 0;
    virtual void clearLabel() = 0;

    virtual std::vector<std::string> getLabelLanguages() const = 0;
    virtual std::string getLanguageLabel(const std::string& code) const = 0;
    virtual void clearLanguageLabel() = 0;
    virtual void setLanguageLabel(const std::string& code, const std::string& label) = 0;
    virtual void removeLanguageLabel(const std::string& code) = 0;


    virtual std::string getDescription() const = 0;
    virtual void setDescription(const std::string& description) = 0;
    virtual bool hasDescription() const = 0;
    virtual void clearDescription() = 0;

    virtual std::vector<std::string> getDescriptionLanguages() const = 0;
    virtual std::string getDescriptionLanguage(const std::string& code) const = 0;
    virtual void clearDescriptionLanguage() = 0;
    virtual void setDescriptionLanguage(const std::string& code, const std::string& label) = 0;
    virtual void removeDescriptionLanguage(const std::string& code) = 0;


    virtual std::string getTags() const = 0;
    virtual void setTags(const std::string& tags) = 0;
    virtual bool hasTags() const = 0;
    virtual void clearTags() = 0;

    virtual int32_t getOrder() const = 0;
    virtual void setOrder(const int32_t order) = 0;
    virtual bool hasOrder() const = 0;
    virtual void clearOrder() = 0;

    virtual std::weak_ptr<IGroupParameter> getParent() const = 0;
    virtual IdType getParentId() const = 0;
    virtual void setParent(IGroupParameterPtr) = 0;
    virtual bool hasParent() const = 0;
    virtual void removeFromParent() = 0;

    virtual std::vector<char> getUserdata() const = 0;
    virtual void setUserdata(const std::vector<char>& userdata) = 0;
    virtual bool hasUserdata() const = 0;
    virtual void clearUserdata() = 0;

    virtual std::string getUserid() const = 0;
    virtual void setUserid(const std::string& userid) = 0;
    virtual bool hasUserid() const = 0;
    virtual void clearUserid() = 0;

    virtual bool getReadonly() const = 0;
    virtual void setReadonly(const bool readonly) = 0;
    virtual bool hasReadonly() const = 0;
    virtual void clearReadonly() = 0;


    // enabled - v1.0 feature here?
    // no maybe not



    virtual bool isGroup() const { return false; }
    virtual bool isValueParameter() const { return false; }

    virtual void setManager(ParameterManagerPtr manager) = 0;
    virtual void setDirty() = 0;    


    virtual void parseTypeOptions(std::istream& is) = 0;
    virtual void parseElementTypeOptions(std::istream& is) = 0;
    virtual bool handleOption(uint8_t option, std::istream& is) { return false; }

    virtual void writeTypeDefinition(Writer &out, bool all) = 0;
    virtual void writeTypeMandatory(Writer &out) = 0;

    virtual void writeValue(Writer &out) {}
    virtual void writeValueOption(Writer &out, bool all, uint8_t id) {}
    virtual bool hasValueOption() { return false; }

    // virtual void writeOtherOptions(Writer &out, bool all) {}

    virtual void update(IParameterPtr other) = 0;



    virtual bool anyOptionChanged() = 0;
    virtual void setAllUnchanged() = 0;
    virtual bool onlyValueChanged() = 0;

    virtual std::recursive_mutex& mutex() const = 0;

    virtual void addUpdatedCb(void* obj, const std::function<void()>& fn) = 0;
    virtual void removeUpdatedCb(void* obj) = 0;
    virtual void clearUpdatedCb() = 0;


protected:
    virtual bool setParentInternal(IGroupParameterPtr) = 0;
    virtual void clearParentInternal() = 0;



public:
    // IParameter()
    // {}

    IParameter(IdType id, const std::string& lbl = "")
        : m_id(id)
        , label(lbl)
    {
#ifdef DO_MEMORY_TEST
        MemoryTools::countUp();
#endif
    }

#ifdef DO_MEMORY_TEST
    ~IParameter()
    {
        MemoryTools::countDown();
    }
#endif

    friend class v010::ParameterParser;
    friend class v010::ParameterWriter;
    friend class v10::ParameterParser;
    friend class v10::ParameterWriter;
    friend class GroupParameter;





    IdType m_id{0};

    // optional fields
    std::string label{};
    std::map<std::string, std::string> languageLabel;
    bool m_hasLabel{false};
    bool labelChanged{false};

    std::string description{};
    std::map<std::string, std::string> languageDescription;
    bool m_hasDescription{false};
    bool descriptionChanged{false};

    Option<std::string> tags{};
    Option<int32_t> order{};

    std::weak_ptr<IGroupParameter> m_parent;
    bool parentChanged{false};

    //TODO: PARAMETER_WIDGET

    std::vector<char> m_userdata;
    bool userdataChanged{false};

    Option<std::string> m_userid{};
    Option<bool> readonly{};


    std::set<uint8_t> m_changed;
};




}

#endif // IPARAMETER_H
