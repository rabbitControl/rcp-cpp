/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2024
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef IPARAMETER_H
#define IPARAMETER_H

#include <vector>
#include <string>
#include <functional>

#include "types.h"
#include "writeable.h"
#include "optionparser.h"
#include "typedefinition.h"
#include "parameter_listener.h"

namespace rcp {

class GroupParameter;
class ITypeDefinition;
class IParameter;
class IParameterManager;

typedef std::shared_ptr<IParameter> ParameterPtr;

class IParameter
    : public Writeable
    , public IOptionparser
{

public:
    virtual ~IParameter();

    //--------------------------------
    // mandatory
    virtual int16_t getId() const = 0;
    virtual ITypeDefinition& getTypeDefinition() const = 0;
    virtual datatype_t getDatatype() const = 0;

    virtual void writeUpdateValue(Writer& out) const = 0;

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

    virtual std::weak_ptr<GroupParameter>& getParent() const = 0;
    virtual bool hasParent() const = 0;
    virtual void removeFromParent() = 0;

    virtual std::vector<char> getUserdata() const = 0;
    virtual void setUserdata(const std::vector<char> userdata) = 0;
    virtual bool hasUserdata() const = 0;
    virtual void clearUserdata() = 0;

    // user id
    virtual std::string getUserid() const = 0;
    virtual void setUserid(const std::string& userid) = 0;
    virtual bool hasUserid() const = 0;
    virtual void clearUserid() = 0;

    // readonly
    virtual bool getReadonly() const = 0;
    virtual void setReadonly(const bool userid) = 0;
    virtual bool hasReadonly() const = 0;
    virtual void clearReadonly() = 0;

    // update parameter from other parameter
    virtual void update(const ParameterPtr& other) = 0;
    virtual ParameterPtr getShared() = 0;

    // listener
    virtual void addUpdateListener(ParameterListener* listener) = 0;
    virtual void removeUpdateListener(ParameterListener* listener) = 0;

    // update callbacks
    virtual const std::function< void() >& addUpdatedCb(std::function< void() >& func) = 0;
    virtual const std::function< void() >& addUpdatedCb(std::function< void() >&& func) = 0;
    virtual void removeUpdatedCb(const std::function< void() >& func) = 0;
    virtual void clearUpdatedCb() = 0;

    //
    virtual void dump() = 0;
    virtual bool waitForParent() const = 0;

    //
    virtual bool isValueParameter() const { return false; }

    template<typename, datatype_t, td_types> friend class TypeDefinition;
    friend class GroupParameter;
    friend class ParameterManager;
    friend class ParameterParser;
    friend class ParameterClient;
    friend class ParameterServer;

protected:
    virtual void setDirty() = 0;
    virtual bool onlyValueChanged() const = 0;

private:
    virtual bool setParentInternal(std::shared_ptr<GroupParameter> parent) = 0;
    virtual void setParent(std::shared_ptr<GroupParameter> parent) = 0;
    virtual void clearParentInternal() = 0;
    virtual void clearParent() = 0;
    virtual void setManager(std::shared_ptr<IParameterManager> manager) = 0;
    virtual void setAllUnchanged() = 0;
    virtual void setParentUnchanged() = 0;
};


template <typename T,
         typename TD>
class IValueParameter
{
public:
    // mandatory
    virtual TD& getDefaultTypeDefinition() = 0;
    virtual const TD& getDefaultTypeDefinition() const = 0;

    // optional
    virtual T getValue() const = 0;
    virtual bool setValue(const T& value) = 0;
    virtual bool hasValue() const = 0;
    virtual void clearValue() = 0;

    // update callbacks
    virtual const std::function< void ( T& )>& addValueUpdatedCb(std::function< void(T&) >& func) = 0;
    virtual void removeValueUpdatedCb(const std::function< void(T&) >& func) = 0;
    virtual void clearValueUpdatedCb() = 0;
};

class IElementParameter
{
public:
    virtual datatype_t getElementType() = 0;
};
}

#endif // IPARAMETER_H
