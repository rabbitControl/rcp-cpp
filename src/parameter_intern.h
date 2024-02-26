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

#ifndef RCP_PARAMETER_INTERN_H
#define RCP_PARAMETER_INTERN_H

#include <cinttypes>
#include <iostream>
#include <istream>
#include <string>
#include <map>
#include <vector>
#include <functional>

#include "stream_tools.h"
#include "iparameter.h"
#include "iparametermanager.h"

#include "type_noopt.h"
#include "type_default.h"
#include "type_number.h"
#include "type_enum.h"
#include "type_uri.h"
#include "type_range.h"
#include "type_string.h"
#include "type_custom.h"
#include "type_array.h"
#include "tinystring.h"


namespace rcp {

    class ParameterManager;
    class GroupParameter;
    typedef std::shared_ptr<GroupParameter> GroupParameterPtr;
    class ParameterFactory;

    static const std::string language_string_any("any");

    //----------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------
    /*
        88888888ba      db         88888888ba          db         88b           d88  88888888888  888888888888  88888888888  88888888ba
        88      "8b    d88b        88      "8b        d88b        888b         d888  88                88       88           88      "8b
        88      ,8P   d8'`8b       88      ,8P       d8'`8b       88`8b       d8'88  88                88       88           88      ,8P
        88aaaaaa8P'  d8'  `8b      88aaaaaa8P'      d8'  `8b      88 `8b     d8' 88  88aaaaa           88       88aaaaa      88aaaaaa8P'
        88""""""'   d8YaaaaY8b     88""""88'       d8YaaaaY8b     88  `8b   d8'  88  88"""""           88       88"""""      88""""88'
        88         d8""""""""8b    88    `8b      d8""""""""8b    88   `8b d8'   88  88                88       88           88    `8b
        88        d8'        `8b   88     `8b    d8'        `8b   88    `888'    88  88                88       88           88     `8b
        88       d8'          `8b  88      `8b  d8'          `8b  88     `8'     88  88888888888       88       88888888888  88      `8b
     */
    //----------------------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------------------
    template <typename TD>
    class Parameter : public IParameter, public std::enable_shared_from_this<IParameter>
    {
    public:
        // static
        static std::shared_ptr< Parameter<TD> > create(int16_t id) {
            return std::make_shared< Parameter<TD> >(id);
        }

    public:
        Parameter(const Parameter<TD>& v)
            : obj(v.obj)
            , m_waitForParent(v.m_waitForParent)
        {}

        Parameter(int16_t id)
            : obj(std::make_shared<Value>(id, TD(*this)))
            , m_waitForParent(false)
        {}


        //------------------------------------
        // Writeable
        void write(Writer& out, bool all) override
        {
            out.write(getId());
            getTypeDefinition().write(out, all);

            writeOptions(out, all);

            // terminator
            out.write(static_cast<char>(TERMINATOR));
        }

        void writeUpdateValue(Writer& out) const override
        {
            out.write(getId());
            getTypeDefinition().writeMandatory(out);
        }


        //------------------------------------
        // IOptionparser
        void parseOptions(std::istream& is) override
        {
            while (!is.eof()) {

                // read option prefix
                parameter_options_t opt = static_cast<parameter_options_t>(is.get());

                // check stream
                CHECK_STREAM

                if (opt == TERMINATOR) {
                    break;
                }


                switch (opt) {
                case PARAMETER_OPTIONS_LABEL: {

                    // peek one
                    int peek = is.peek();
                    CHECK_STREAM

                    char code[4];
                    code[3] = 0;
                    while (peek > 0) {

                        is.read(code, 3);

                        CHECK_STREAM

                        std::string code_str(code);
                        std::string label = readTinyString(is);

                        CHECK_STREAM

                        if (code_str == language_string_any) {
                            obj->hasLabel = true;
                            obj->label = label;
                        } else {
                            obj->languageLabel[code_str] = label;
                        }

                        // peek next byte
                        peek = is.peek();
                    }

                    CHECK_STREAM

                    // read 0-byte off the stream
                    is.get();
                    break;
                }
                case PARAMETER_OPTIONS_DESCRIPTION: {

                    // peek one
                    int peek = is.peek();
                    CHECK_STREAM

                    char code[4];
                    code[3] = 0;
                    while (peek > 0) {

                        is.read(code, 3);                       

                        CHECK_STREAM

                        std::string code_str(code);
                        std::string description = readShortString(is);

                        CHECK_STREAM

                        if (code_str == language_string_any) {
                            obj->hasDescription = true;
                            obj->description = description;
                        } else {
                            obj->languageDescription[code_str] = description;
                        }

                        // peek next byte
                        peek = is.peek();
                    }

                    CHECK_STREAM
                    // read 0-byte off the stream
                    is.get();
                    break;
                }
                case PARAMETER_OPTIONS_TAGS: {
                    std::string st = readTinyString(is);
                    CHECK_STREAM

                    obj->tags = st;
                    break;
                }
                case PARAMETER_OPTIONS_ORDER: {

                    int32_t val = readFromStream(is, obj->order.value());
                    CHECK_STREAM

                    obj->order = val;
                    break;
                }
                case PARAMETER_OPTIONS_PARENTID: {

                    int16_t parent_id = readFromStream(is, parent_id);
                    CHECK_STREAM


                    if (auto manager = obj->parameterManager.lock())
                    {
                        if (parent_id != 0)
                        {
                            ParameterPtr parent = manager->getParameter(parent_id);
                            if (parent)
                            {
                                // NOTE:
                                // parsed parameter is is a proxy parameter
                                // _do_not_ set this paremter as a child of the parent!
                                // a proxy-parameter is either updating an exisitng parameter in cache
                                // or is added. if the proxy-parameter is added it gets added to the parent too
                                if (parent->getDatatype() == DATATYPE_GROUP)
                                {
                                    if (auto p = obj->parent.lock())
                                    {
                                        if (parent->getId() != p->getId())
                                        {
                                            // remove parameter-id from parent...
                                            p->removeChild(shared_from_this());
                                        }
                                    }

                                    obj->parent = std::dynamic_pointer_cast<GroupParameter>(parent);
                                }
                                else
                                {
                                    // parent not a group!
                                }
                            }
                            else
                            {
                                // put it into list, look parent up later
                                m_waitForParent = true;
                                manager->addMissingParent(parent_id, shared_from_this());
                            }
                        }
                        else
                        {
                            // set root group as parent
                            obj->parent = std::dynamic_pointer_cast<GroupParameter>(manager->getRootGroup());
                        }
                    }
                    else
                    {
                        // TODO: manager not set... look parent up later?
                        std::cerr << "no manager set!\n";
                        std::flush(std::cerr);
                    }


                    break;
                }

                case PARAMETER_OPTIONS_WIDGET:
                    // TODO: handle this
                    break;
                case PARAMETER_OPTIONS_USERDATA:
                {
                    std::uint32_t size = readFromStream(is, size);
                    std::vector<char> data;
                    obj->userdata.resize(size);
                    is.read(obj->userdata.data(), size);
                    break;
                }
                case PARAMETER_OPTIONS_USERID: {

                    std::string st = readTinyString(is);
                    CHECK_STREAM

                    obj->userid = st;
                    break;
                }

                case PARAMETER_OPTIONS_READONLY: {

                    bool readonly = readFromStream(is, readonly);
                    CHECK_STREAM

                    obj->readonly = readonly;
                    break;
                }

                // handle all other cases in "handleOption"
//                case PARAMETER_OPTIONS_VALUE:
                default:
                    if (!handleOption(opt, is)) {
                        // error!
                        return;
                    }
                    break;
                }

            } // while
        } // parseOptions


        //------------------------------------
        // implement IParameter

        ParameterPtr getShared() override
        {
            return shared_from_this();
        }

        bool waitForParent() const override
        {
            return m_waitForParent;
        }

        int16_t getId() const override
        {
            return obj->parameter_id;
        }

        ITypeDefinition& getTypeDefinition() const override
        {
            return obj->typeDefinition;
        }

        datatype_t getDatatype() const override
        {
            return getTypeDefinition().getDatatype();
        }

        // optional
        std::string getLabel() const override
        {
            return obj->label;
        }
        void setLabel(const std::string& label) override
        {
            if (obj->setLabel(label))
            {
                setDirty();
            }
        }
        bool hasLabel() const override
        {
            return obj->hasLabel;
        }
        void clearLabel() override
        {
            obj->hasLabel = false;
            setDirty();
        }

        // label languages
        std::vector<std::string> getLabelLanguages() const override
        {
            std::vector<std::string> keys;
            for(auto it = obj->languageLabel.begin(); it != obj->languageLabel.end(); ++it) {
              keys.push_back(it->first);
            }

            return keys;
        }

        std::string getLanguageLabel(const std::string& code) const override
        {
            std::map<std::string, std::string>::iterator it = obj->languageLabel.find(code);
            if (it != obj->languageLabel.end()) {
                return it->second;
            }

            return empty_string;
        }

        void clearLanguageLabel() override
        {
            obj->languageLabel.clear();
            obj->labelChanged = true;
            setDirty();
        }

        void setLanguageLabel(const std::string& code, const std::string& label) override
        {
            if (obj->setLanguageLabel(code, label))
            {
                setDirty();
            }
        }

        void removeLanguageLabel(const std::string& code) override
        {
            auto it = obj->languageLabel.find(code);
            if (it != obj->languageLabel.end()) {
                obj->languageLabel.erase(it);

                obj->labelChanged = true;
                setDirty();
            }
        }

        std::string getDescription() const override
        {
            return obj->description;
        }
        void setDescription(const std::string& description) override
        {
            if (obj->setDescription(description))
            {
                setDirty();
            }
        }
        bool hasDescription() const override
        {
            return obj->hasDescription;
        }
        void clearDescription() override
        {
            obj->hasDescription = false;
            setDirty();
        }

        // description languages
        std::vector<std::string> getDescriptionLanguages() const override
        {
            std::vector<std::string> keys;
            for(auto it = obj->languageDescription.begin(); it != obj->languageDescription.end(); ++it) {
              keys.push_back(it->first);
            }

            return keys;
        }

        std::string getDescriptionLanguage(const std::string& code) const override
        {
            std::map<std::string, std::string>::iterator it = obj->languageDescription.find(code);
            if (it != obj->languageDescription.end()) {
                return it->second;
            }

            return empty_string;
        }

        void clearDescriptionLanguage() override
        {
            obj->languageDescription.clear();
            obj->descriptionChanged = true;
            setDirty();
        }

        void setDescriptionLanguage(const std::string& code, const std::string& description) override
        {
            if (obj->setLanguageDescription(code, description))
            {
                setDirty();
            }
        }

        void removeDescriptionLanguage(const std::string& code) override
        {
            auto it = obj->languageDescription.find(code);
            if (it != obj->languageDescription.end()) {
                obj->languageDescription.erase(it);

                obj->descriptionChanged = true;
                setDirty();
            }
        }

        //----------------------
        // tags
        std::string getTags() const override
        {
            return obj->tags.value();
        }
        void setTags(const std::string& tags) override
        {
            if (obj->setTags(tags))
            {
                setDirty();
            }
        }
        bool hasTags() const override
        {
            return obj->tags.hasValue();
        }
        void clearTags() override
        {
            obj->tags.clearValue();
            if (obj->tags.changed())
            {
                setDirty();
            }
        }


        //----------------------
        // order
        int32_t getOrder() const override
        {
            if (obj->order.hasValue())
            {
                return obj->order.value();
            }

            return 0;
        }
        void setOrder(const int32_t order) override
        {
            if (obj->setOrder(order))
            {
                setDirty();
            }
        }
        bool hasOrder() const override
        {
            return obj->order.hasValue();
        }
        void clearOrder() override
        {
            obj->order.clearValue();
            if (obj->order.changed())
            {
                setDirty();
            }
        }

        //----------------------
        // parent
        virtual std::weak_ptr<GroupParameter>& getParent() const override;
        bool hasParent() const override
        {
            return obj->parent.lock() != nullptr;
        }

        virtual void removeFromParent() override;


        //----------------------
        // userdata
        std::vector<char> getUserdata() const override
        {
            return obj->userdata;
        }
        void setUserdata(const std::vector<char> userdata) override
        {
            if (obj->setUserData(userdata))
            {
                setDirty();
            }
        }
        bool hasUserdata() const override
        {
            return !obj->userdata.empty();
        }
        void clearUserdata() override
        {
            if (!obj->userdata.empty())
            {
                obj->userdata.clear();
                obj->userdataChanged = true;
                setDirty();
            }
            else
            {
                obj->userdataChanged = false;
            }
        }

        //----------------------
        // userid
        std::string getUserid() const override
        {
            return obj->userid.value();
        }
        void setUserid(const std::string& userid) override
        {
            if (obj->setUserid(userid))
            {
                setDirty();
            }
        }
        bool hasUserid() const override
        {
            return obj->userid.hasValue();
        }
        void clearUserid() override
        {
            obj->userid.clearValue();
            if (obj->userid.changed())
            {
                setDirty();
            }
        }

        //----------------------
        // readonly
        bool getReadonly() const override
        {
            if (obj->readonly.hasValue())
            {
                return obj->readonly.value();
            }

            return false;
        };
        void setReadonly(const bool readonly) override
        {
            if (obj->setReadonly(readonly))
            {
                setDirty();
            }
        }
        bool hasReadonly() const override
        {
            return obj->readonly.hasValue();
        };
        void clearReadonly() override
        {
            obj->readonly.clearValue();
            if (obj->readonly.changed())
            {
                setDirty();
            }
        }

        //----------------------
        //
        void dump() override;

        // update from proxy-parameter
        void update(const ParameterPtr& other) override;

        // update callbacks
        const std::function< void() >& addUpdatedCb(std::function< void() >& func) override
        {
            for(auto& f : obj->updatedCallbacks)
            {
                if (&func == &f->callback) {
                    // already contained
                    return f->callback;
                }
            }

            UpdateEventHolderPtr event = std::make_shared<UpdateEventHolder>(func);
            obj->updatedCallbacks.push_back(std::move(event));
            return obj->updatedCallbacks.back()->callback;
        }

        const std::function< void() >& addUpdatedCb(std::function< void() >&& func) override
        {
            for(auto& f : obj->updatedCallbacks)
            {
                if (&func == &f->callback) {
                    // already contained
                    return f->callback;
                }
            }

            UpdateEventHolderPtr event = std::make_shared<UpdateEventHolder>(func);
            obj->updatedCallbacks.push_back(std::move(event));
            return obj->updatedCallbacks.back()->callback;
        }

        void removeUpdatedCb(const std::function< void() >& func) override
        {
            for(auto it = obj->updatedCallbacks.begin(); it != obj->updatedCallbacks.end(); it++ )
            {
                if (&func == &(it->get()->callback)) {
                    obj->updatedCallbacks.erase(it);
                    break;
                }
            }
        }

        void clearUpdatedCb() override
        {
            obj->updatedCallbacks.clear();
        }


        // listener

        void addUpdateListener(ParameterListener* listener) override
        {
            if (std::find(obj->m_parameterListener.begin(), obj->m_parameterListener.end(), listener) == obj->m_parameterListener.end())
            {
                obj->m_parameterListener.push_back(listener);
            }
        }

        void removeUpdateListener(ParameterListener* listener) override
        {
            auto it = std::find(obj->m_parameterListener.begin(), obj->m_parameterListener.end(), listener);
            if (it != obj->m_parameterListener.end())
            {
                obj->m_parameterListener.erase(it);
            }
        }



        friend class ParameterManager;
        friend class GroupParameter;
        friend class ParameterFactory;
        template<typename, typename, datatype_t> friend class ValueParameter;

    protected:
        virtual bool handleOption(const parameter_options_t& /*opt*/, std::istream& /*is*/) {
            return false;
        }

        TD& getRealTypeDef() { return obj->typeDefinition; }
        const TD& getRealTypeDef() const { return obj->typeDefinition; }

        // IParameter
        void setDirty() override
        {
            if (auto manager = obj->parameterManager.lock()) {
                manager->setParameterDirty(shared_from_this());
            }
        }

        bool onlyValueChanged() const override
        {
            return !anyOptionChanged() && !getTypeDefinition().anyOptionChanged();
        }

        bool anyOptionChanged() const {
            return obj->labelChanged
                    || obj->descriptionChanged
                    || obj->tags.changed()
                    || obj->order.changed()
                    || obj->parentChanged
                    || obj->userdataChanged
                    || obj->userid.changed()
                    || obj->readonly.changed();
        }

        void writeOptions(Writer& out, bool all) {
            // writing options
            obj->write(out, all);
        }

    private:

        virtual bool setParentInternal(GroupParameterPtr parent) override;
        virtual void setParent(GroupParameterPtr parent) override;
        virtual void clearParentInternal() override
        {
            obj->parent.reset();
            obj->parentChanged = true;
            m_waitForParent = false;
        }
        virtual void clearParent() override
        {
            clearParentInternal();
            setDirty();
        }

        void setManager(std::shared_ptr<IParameterManager> manager) override
        {
            obj->parameterManager = manager;            
        }

        class UpdateEventHolder {
        public:
            UpdateEventHolder(std::function< void() >&& cb) : callback(std::move(cb)) {}
            UpdateEventHolder(std::function< void() >& cb) : callback(cb) {}
            const std::function< void() > callback;
        };
        typedef std::shared_ptr<UpdateEventHolder> UpdateEventHolderPtr;


        void setAllUnchanged() override
        {
            obj->labelChanged = false;
            obj->descriptionChanged = false;
            obj->tags.setUnchanged();
            obj->order.setUnchanged();
            obj->parentChanged = false;
            obj->userdataChanged = false;
            obj->userid.setUnchanged();

            getTypeDefinition().setAllUnchanged();
        }

        void setParentUnchanged() override
        {
            obj->parentChanged = false;
        }


        class Value {
        public:
            Value(int16_t id, const TD& td) :
                parameter_id(id)
              , typeDefinition(td)
            {
                // init vars
                label = empty_string;
                hasLabel = false;
                labelChanged = false;

                description = empty_string;
                hasDescription = false;
                descriptionChanged = false;

                tags.clearValue();
                order.clearValue();

                parentChanged = false;

                userdata.clear();
                userdataChanged = false;

                userid.clearValue();
            }

            // write label and all language labels
            void writeLabel(Writer& out) {

                out.write(static_cast<char>(PARAMETER_OPTIONS_LABEL));

                if (hasLabel) {

                    out.write(language_string_any, false);
                    out.writeTinyString(label);
                }

                for (auto& l : languageLabel) {

                    if (l.first.length() < 3) {
                        continue;
                    }

                    out.write(l.first.substr(0, 3), false);
                    out.writeTinyString(l.second);
                }

                out.write(static_cast<char>(TERMINATOR));
            }

            // write description and language description
            void writeDescription(Writer& out) {

                out.write(static_cast<char>(PARAMETER_OPTIONS_DESCRIPTION));

                if (hasDescription) {

                    out.write(language_string_any, false);
                    out.writeShortString(description);
                }

                for (auto& l : languageDescription) {

                    if (l.first.length() < 3) {
                        continue;
                    }

                    out.write(l.first.substr(0, 3), false);
                    out.writeShortString(l.second);
                }

                out.write(static_cast<char>(TERMINATOR));
            }


            // write all options
            void write(Writer& out, bool all) {

                // label
                if (hasLabel || languageLabel.size() > 0) {

                    if (all || labelChanged) {

                        writeLabel(out);

                        if (!all) {
                            labelChanged = false;
                        }
                    }
                } else if(labelChanged) {

                    out.write(static_cast<char>(PARAMETER_OPTIONS_LABEL));
                    out.write(static_cast<char>(0));
                    labelChanged = false;
                }


                // description
                if (hasDescription || languageDescription.size() > 0) {

                    if (all || descriptionChanged) {

                        writeDescription(out);

                        if (!all) {
                            descriptionChanged = false;
                        }
                    }
                } else if (descriptionChanged) {

                    out.write(static_cast<char>(PARAMETER_OPTIONS_DESCRIPTION));
                    out.write(static_cast<char>(0));
                    descriptionChanged = false;
                }


                // tags
                if (tags.hasValue()) {

                    if (all || tags.changed()) {
                        out.write(static_cast<char>(PARAMETER_OPTIONS_TAGS));
                        out.writeTinyString(tags.value());

                        if (!all) {
                            tags.setUnchanged();
                        }
                    }
                } else if (tags.changed()) {

                    out.write(static_cast<char>(PARAMETER_OPTIONS_TAGS));
                    out.writeTinyString("");
                    tags.setUnchanged();
                }

                // order
                if (order.hasValue()) {

                    if (all || order.changed()) {
                        out.write(static_cast<char>(PARAMETER_OPTIONS_ORDER));
                        out.write(order.value());

                        if (!all) {
                            order.setUnchanged();
                        }
                    }
                } else if (order.changed()) {

                    out.write(static_cast<char>(PARAMETER_OPTIONS_ORDER));
                    out.write(static_cast<int32_t>(0));
                    order.setUnchanged();
                }


                // parent id
                if (std::shared_ptr<GroupParameter> p = parent.lock()) {

                    if (all || parentChanged) {
                        out.write(static_cast<char>(PARAMETER_OPTIONS_PARENTID));

                        // cast to IParameter* due to incomplete type
                        out.write(((IParameter*)p.get())->getId());

                        if (!all) {
                            parentChanged = false;
                        }
                    }
                } else if (parentChanged) {

                    out.write(static_cast<char>(PARAMETER_OPTIONS_PARENTID));
                    out.write(static_cast<int16_t>(0));
                    parentChanged = false;
                }


                // TODO: widget

                // userdata
                if (!userdata.empty())
                {
                    if (all || userdataChanged)
                    {
                        out.write(static_cast<char>(PARAMETER_OPTIONS_USERDATA));
                        out.write(static_cast<uint32_t>(userdata.size()));
                        out.write(userdata.data(), userdata.size());

                        if (!all)
                        {
                            userdataChanged = false;
                        }
                    }
                }
                else if (userdataChanged)
                {
                    out.write(static_cast<char>(PARAMETER_OPTIONS_USERDATA));
                    out.write(static_cast<uint32_t>(0));
                    userdataChanged = false;
                }


                // userid
                if (userid.hasValue()) {

                    if (all || userid.changed()) {
                        out.write(static_cast<char>(PARAMETER_OPTIONS_USERID));
                        out.writeTinyString(userid.value());

                        if (!all) {
                            userid.setUnchanged();
                        }
                    }
                } else if (userid.changed()) {

                    out.write(static_cast<char>(PARAMETER_OPTIONS_USERID));
                    out.writeTinyString("");
                    userid.setUnchanged();
                }


                // readonly
                if (readonly.hasValue()) {

                    if (all || readonly.changed()) {
                        out.write(static_cast<char>(PARAMETER_OPTIONS_READONLY));
                        out.write(readonly.value());

                        if (!all) {
                            readonly.setUnchanged();
                        }
                    }
                } else if (readonly.changed()) {

                    out.write(static_cast<char>(PARAMETER_OPTIONS_READONLY));
                    out.write(false);
                    readonly.setUnchanged();
                }
            }


            bool setLabel(const std::string& newLabel)
            {
                if (label != newLabel)
                {
                    std::string old_value = label;

                    label = newLabel;
                    hasLabel = true;
                    labelChanged = true;

                    // call listener
                    for (auto listener : m_parameterListener)
                    {
                        listener->onLabelChanged(label, old_value);
                    }

                    return true;
                }

                return false;
            }

            bool setLanguageLabel(const std::string& lang, const std::string& newLabel)
            {
                std::string old_value = "";

                if (languageLabel.find(lang) != languageLabel.end())
                {
                    // found key
                    old_value = languageLabel[lang];
                }

                if (newLabel != old_value)
                {
                    languageLabel[lang] = newLabel;
                    labelChanged = true;

                    // call listener
                    for (auto listener : m_parameterListener)
                    {
                        listener->onLanguageLabelChanged(lang, newLabel, old_value);
                    }

                    return true;
                }

                return false;
            }

            bool setDescription(const std::string& newDescription)
            {
                if (description != newDescription)
                {
                    std::string old_value = description;

                    description = newDescription;
                    hasDescription = true;
                    descriptionChanged = true;

                    // call listener
                    for (auto listener : m_parameterListener)
                    {
                        listener->onDescriptionChanged(description, old_value);
                    }

                    return true;
                }

                return false;
            }

            bool setLanguageDescription(const std::string& lang, const std::string& newDescription)
            {
                std::string old_value = "";

                if (languageDescription.find(lang) != languageDescription.end())
                {
                    // found key
                    old_value = languageDescription[lang];
                }

                if (newDescription != old_value)
                {
                    languageDescription[lang] = newDescription;
                    descriptionChanged = true;

                    // call listener
                    for (auto listener : m_parameterListener)
                    {
                        listener->onLanguageDescriptionChanged(lang, newDescription, old_value);
                    }

                    return true;
                }

                return false;
            }

            bool setTags(const std::string& newTags)
            {
                std::string old_value = tags.value();
                tags = newTags;
                if (tags.changed())
                {
                    // call listener
                    for (auto listener : m_parameterListener)
                    {
                        listener->onTagsChanged(tags.value(), old_value);
                    }
                }

                return tags.changed();
            }

            bool setOrder(int32_t newOrder)
            {
                int32_t old_value = order.value();
                order = newOrder;
                if (order.changed())
                {
                    // call listener
                    for (auto listener : m_parameterListener)
                    {
                        listener->onOrderChanged(order.value(), old_value);
                    }
                }

                return order.changed();
            }

            bool setUserData(const std::vector<char>& newUserdata)
            {
                std::vector<char> old_value = userdata;

                // always changes
                userdata = newUserdata;
                userdataChanged = true;

                // call listener
                for (auto listener : m_parameterListener)
                {
                    listener->onUserdataChanged(userdata, old_value);
                }

                return true;
            }

            bool setUserid(const std::string& newUserid)
            {
                std::string old_value = userid.value();

                userid = newUserid;

                if (userid.changed())
                {
                    // call listener
                    for (auto listener : m_parameterListener)
                    {
                        listener->onUseridChanged(userid.value(), old_value);
                    }
                }

                return userid.changed();
            }

            bool setReadonly(bool newReadonly)
            {
                readonly = newReadonly;

                if (readonly.changed())
                {
                    // call listener
                    for (auto listener : m_parameterListener)
                    {
                        listener->onReadonlyChanged(readonly.value());
                    }
                }

                return readonly.changed();
            }


            void callUpdatedCb() {
                for (auto& event : updatedCallbacks) {
                    event->callback();
                }
            }

            // mandatory
            int16_t parameter_id;
            TD typeDefinition;

            // optional fields
            std::string label{};
            std::map<std::string, std::string> languageLabel;
            bool hasLabel;
            bool labelChanged;

            std::string description{};
            std::map<std::string, std::string> languageDescription;
            bool hasDescription;
            bool descriptionChanged;

            Option<std::string> tags{};
            Option<int32_t> order{};

            std::weak_ptr<GroupParameter> parent;
            bool parentChanged;

            //TODO: PARAMETER_WIDGET

            std::vector<char> userdata;
            bool userdataChanged;

            Option<std::string> userid{};
            Option<bool> readonly{};

            std::vector< std::shared_ptr<UpdateEventHolder> > updatedCallbacks;
            std::weak_ptr<IParameterManager> parameterManager;
            std::vector<ParameterListener*> m_parameterListener;
        };

        std::shared_ptr<Value> obj;
        bool m_waitForParent;
    };



    //--------------------------------------------------------------------
    //--------------------------------------------------------------------
    /*   
        8b           d8   db         88          88        88  88888888888
        `8b         d8'  d88b        88          88        88  88
         `8b       d8'  d8'`8b       88          88        88  88
          `8b     d8'  d8'  `8b      88          88        88  88aaaaa
           `8b   d8'  d8YaaaaY8b     88          88        88  88"""""
            `8b d8'  d8""""""""8b    88          88        88  88
             `888'  d8'        `8b   88          Y8a.    .a8P  88
              `8'  d8'          `8b  88888888888  `"Y8888Y"'   88888888888
     */
    //--------------------------------------------------------------------
    //--------------------------------------------------------------------
    template<
        typename T,
        typename TD,
        datatype_t type_id
    >
    class ValueParameter : public Parameter<TD>, public IValueParameter<T, TD>
    {
    public:
        static std::shared_ptr< ValueParameter<T, TD, type_id> > create(int16_t id) {
            return std::make_shared< ValueParameter<T, TD, type_id> >(id);
        }
        static std::shared_ptr< ValueParameter<T, TD, type_id> > create(int16_t id, const T& init) {
            return std::make_shared< ValueParameter<T, TD, type_id> >(id, init);
        }


        ValueParameter(int16_t id) :
            Parameter<TD>(id)
          , obj(std::make_shared<Value>())
        {}

        ValueParameter(int16_t id, T& init) :
            Parameter<TD>(id)
          , obj(std::make_shared<Value>())
        {
            // NOTE: just set value internally
            obj->value = init;
            obj->value.clearValue();
            obj->value.setUnchanged();
        }

        ValueParameter(int16_t id, const T& init) :
            Parameter<TD>(id)
          , obj(std::make_shared<Value>())
        {
            // NOTE: just set value internally
            obj->value = init;
            obj->value.clearValue();
            obj->value.setUnchanged();
        }

        // copy constructor
        ValueParameter(const ValueParameter<T, TD, type_id>& v) :
            Parameter<TD>(v)
          , obj(v.obj)
        {}

        ~ValueParameter()
        {}

        //------------------------------------
        // Writeable
        void write(Writer& out, bool all) override
        {
            out.write(Parameter<TD>::getId());
            getDefaultTypeDefinition().write(out, all);

            obj->write(out, all);

            Parameter<TD>::writeOptions(out, all);

            // terminator
            out.write(static_cast<char>(TERMINATOR));
        }

        //------------------------------------
        // iParameter
        void writeUpdateValue(Writer& out) const override
        {
            Parameter<TD>::writeUpdateValue(out);
            obj->writeValue(out);
        }

        void dump() override
        {
            Parameter<TD>::dump();

            if (hasValue()) {
                std::cout << "value: " << value_to_string(obj->value.value()) << "\n";
            }
        }

        bool isValueParameter() const override
        {
            return true;
        }

        virtual bool handleOption(const parameter_options_t& opt, std::istream& is) override
        {
            if (opt == PARAMETER_OPTIONS_VALUE) {

                // read options
                T val = getDefaultTypeDefinition().readValue(is);
                CHECK_STREAM_RETURN(false)

                obj->value = val;
                return true;
            }

            // could not parse... error
            return false;
        }

        void setAllUnchanged() override
        {
            Parameter<TD>::setAllUnchanged();
            obj->value.setUnchanged();
        }


        //---------------------------
        // IValueParameter
        T getValue() const override
        {
            return obj->value.value();
        }
        bool setValue(const T& value) override
        {
            obj->value = value;
            if (obj->value.changed())
            {
                setDirty();
            }
            return obj->value.changed();
        }
        bool hasValue() const override
        {
            return obj->value.hasValue();
        }
        void clearValue() override
        {
            obj->value.clearValue();
            if (obj->value.changed())
            {
                setDirty();
            }
        }

        TD& getDefaultTypeDefinition() override
        {
            return Parameter<TD>::getRealTypeDef();
        }
        const TD& getDefaultTypeDefinition() const override
        {
            return Parameter<TD>::getRealTypeDef();
        }



        //--------------------------------------------
        // convenience - default
        // do that in subclass?

        void setDefault(const T& value) {
            Parameter<TD>::getRealTypeDef().setDefault(value);
        }
        const T getDefault() const {
            return Parameter<TD>::getRealTypeDef().getDefault();
        }


        //--------------------------------------------
        // convenience - number
        // do that in subclass?

        template<class Q = T>
        typename std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value, T>::type
        getMinimum() const
        {
            const TypeDefinition<T, type_id, td_num >& d = Parameter<TD>::getRealTypeDef();
            return d.getMinimum();
        }

        template<class Q = T>
        void setMinimum(const typename std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value, T>::type& value)
        {
            TypeDefinition<T, type_id, td_num >& d = Parameter<TD>::getRealTypeDef();
            d.setMinimum(value);
        }

        template<class Q = T>
        typename std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value, T>::type
        getMaximum() const
        {
            const TypeDefinition<T, type_id, td_num >& d = Parameter<TD>::getRealTypeDef();
            return d.getMaximum();
        }

        template<class Q = T>
        void setMaximum(const typename std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value, T>::type& value)
        {
            TypeDefinition<T, type_id, td_num >& d = Parameter<TD>::getRealTypeDef();
            d.setMaximum(value);
        }

        template<class Q = T>
        typename std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value, T>::type
        getMultipleof() const
        {
            const TypeDefinition<T, type_id, td_num >& d = Parameter<TD>::getRealTypeDef();
            return d.getMultipleof();
        }

        template<class Q = T>
        void setMultipleof(const typename std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value, T>::type& value)
        {
            TypeDefinition<T, type_id, td_num >& d = Parameter<TD>::getRealTypeDef();
            d.setMultipleof(value);
        }


        template<class Q = T,
                 typename = std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value > >
        void setScale(const number_scale_t& scale)
        {
            TypeDefinition<T, type_id, td_num >& d = Parameter<TD>::getRealTypeDef();
            d.setScale(scale);
        }

        template<class Q = T,
                 typename = std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value > >
        number_scale_t getScale() const
        {
            const TypeDefinition<T, type_id, td_num >& d = Parameter<TD>::getRealTypeDef();
            return d.getScale();
        }

        template<class Q = T,
                 typename = std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value > >
        void setUnit(const std::string& unit)
        {
            auto& d = Parameter<TD>::getRealTypeDef();
            d.setUnit(unit);
        }

        template<class Q = T,
                 typename = std::enable_if<std::is_arithmetic<Q>::value && !std::is_same<Q, bool>::value > >
        std::string getUnit() const
        {
            auto& d = Parameter<TD>::getRealTypeDef();
            return d.getUnit();
        }



        //--------------------------------------------
        // convenience - other... string, enum, etc
        // do that in subclasses?

        using Parameter<TD>::getId;
        using Parameter<TD>::getTypeDefinition;

        virtual void update(const ParameterPtr& other) override
        {
            if (other.get() == this) {
                //c'mon, it's me!
                return;
            }

            if (other->getId() != getId()) {
                std::cerr << "trying to update value-parameter from parameter with wrong id!\n";
                return;
            }

            // update value
            auto v_other = std::dynamic_pointer_cast<ValueParameter<T, TD, type_id> >(other);
            if (v_other)
            {
                if (v_other->hasValue())
                {
                    obj->value = v_other->getValue();
                    if (obj->value.changed())
                    {
                        obj->callValueUpdatedCb();
                    }
                }
            }

            // update base
            Parameter<TD>::update(other);
        }


        const std::function< void ( T& )>& addValueUpdatedCb(std::function< void(T&) >& func) override
        {
            for(auto& f : obj->valueUpdatedCallbacks) {
                if (&func == &f->callback) {
                    // already contained
                    return f->callback;
                }
            }

            ValueUpdateEventHolderPtr event = std::make_shared<ValueUpdateEventHolder>(func);
            obj->valueUpdatedCallbacks.push_back(std::move(event));
            return obj->valueUpdatedCallbacks.back()->callback;
        }

        const std::function< void(T&) >& addValueUpdatedCb(std::function< void(T&) >&& func)
        {
            for(auto& f : obj->valueUpdatedCallbacks) {
                if (&func == &f->callback) {
                    // already contained
                    return f->callback;
                }
            }

            ValueUpdateEventHolderPtr event = std::make_shared<ValueUpdateEventHolder>(func);
            obj->valueUpdatedCallbacks.push_back(std::move(event));
            return obj->valueUpdatedCallbacks.back()->callback;
        }

        void removeValueUpdatedCb(const std::function< void(T&) >& func) override
        {
            for(auto it = obj->valueUpdatedCallbacks.begin(); it != obj->valueUpdatedCallbacks.end(); it++ ) {

                if (&func == &(it->get()->callback)) {
                    obj->valueUpdatedCallbacks.erase(it);
                    break;
                }
            }
        }

        void clearValueUpdatedCb() override
        {
            obj->valueUpdatedCallbacks.clear();
        }

        friend class ParameterManager;
        friend class GroupParameter;
        friend class ParameterFactory;

    protected:
        using Parameter<TD>::setDirty;

        virtual bool onlyValueChanged() const override
        {
            return Parameter<TD>::onlyValueChanged()
                    && obj->value.changed();
        }

    private:

        class ValueUpdateEventHolder {
        public:
            ValueUpdateEventHolder(std::function< void(T&) >&& cb) : callback(std::move(cb)) {}
            ValueUpdateEventHolder(std::function< void(T&) >& cb) : callback(cb) {}
            const std::function< void(T&) > callback;
        };
        typedef std::shared_ptr<ValueUpdateEventHolder> ValueUpdateEventHolderPtr;


        class Value {
        public:
            Value()
            {}

            // writing options
            void write(Writer& out, bool all) {

                // value
                if (value.hasValue()) {

                    if (all || value.changed()) {
                        out.write(static_cast<char>(PARAMETER_OPTIONS_VALUE));
                        out.write(value.value());

                        if (!all) {
                            value.setUnchanged();
                        }
                    }
                } else if (value.changed()) {
                    out.write(static_cast<char>(PARAMETER_OPTIONS_VALUE));

                    writeValue(out);
                }
            }

            void writeValue(Writer& out) {
                out.write(value.value());
                value.setUnchanged();
            }

            void callValueUpdatedCb() {
                for (auto& f : valueUpdatedCallbacks) {
                    f->callback(value.value());
                }
            }

            Option<T> value{};

            std::vector< std::shared_ptr<ValueUpdateEventHolder> > valueUpdatedCallbacks;
        };
        std::shared_ptr<Value> obj;

        //
        ValueParameter(std::shared_ptr<Value> obj) :
            obj(obj)
        {}
    };



    /*
      ,ad8888ba,   88888888ba     ,ad8888ba,    88        88  88888888ba
     d8"'    `"8b  88      "8b   d8"'    `"8b   88        88  88      "8b
    d8'            88      ,8P  d8'        `8b  88        88  88      ,8P
    88             88aaaaaa8P'  88          88  88        88  88aaaaaa8P'
    88      88888  88""""88'    88          88  88        88  88""""""'
    Y8,        88  88    `8b    Y8,        ,8P  88        88  88
     Y8a.    .a88  88     `8b    Y8a.    .a8P   Y8a.    .a8P  88
      `"Y88888P"   88      `8b    `"Y8888Y"'     `"Y8888Y"'   88
    */
    class GroupParameter : public Parameter<GroupTypeDefinition>
    {
    public:
//        template<typename> friend class Parameter;

        static GroupParameterPtr create(int16_t id) {
            return std::make_shared<GroupParameter>(id);
        }

        GroupParameter(const GroupParameter& v)
            : Parameter<GroupTypeDefinition>(v)
            , obj(v.obj)
        {}

        GroupParameter()
            : Parameter<GroupTypeDefinition>(static_cast<int16_t>(0))
            , obj(std::make_shared<Value>())
        {}

        GroupParameter(int16_t id)
            : Parameter<GroupTypeDefinition>(id)
            , obj(std::make_shared<Value>())
        {}


        template<typename> friend class Parameter;
        friend class ParameterManager;
        friend class ParameterServer;
        friend class ParameterFactory;

        void addChild(ParameterPtr child);
        std::map<short, ParameterPtr >& children() const;
        void dumpChildren(int indent) const;

    private:
        void removeChild(ParameterPtr child);
        void addChildInternal(ParameterPtr child);


        class Value {
        public:
            Value()
            {}

            std::map<short, ParameterPtr > children;
        };

        std::shared_ptr<Value> obj;

        GroupParameter(std::shared_ptr<Value> obj) :
            Parameter<GroupTypeDefinition>(static_cast<int16_t>(0))
          , obj(obj)
        {}
    };




    template <typename TD>
    void Parameter<TD>::update(const ParameterPtr& other)
    {
        if (other.get() == this)
        {
            // c'mon, it's me!
            return;
        }

        if (other->getId() != getId())
        {
            std::cerr << "trying to update from parameter with wrong id: " << getId() << " != " << other->getId() << "\n";
            return;
        }

        bool updated = false;

        if (other->hasLabel())
        {
            // copy other language labels
            auto o_lang = other->getLabelLanguages();
            if (!o_lang.empty())
            {
                for (std::string& l : o_lang)
                {
                    if (obj->setLanguageLabel(l, other->getLanguageLabel(l)))
                    {
                        updated = true;
                    }
                }
            }

            // default label
            if (obj->setLabel(other->getLabel()))
            {
                updated = true;
            }
        }

        if (other->hasDescription())
        {
            // copy other language descriptions
            auto o_lang = other->getDescriptionLanguages();
            if (!o_lang.empty())
            {
                for (std::string& l : o_lang)
                {
                    if (obj->setLanguageDescription(l, other->getDescriptionLanguage(l)))
                    {
                        updated = true;
                    }
                }
            }

            // default description
            if (obj->setDescription(other->getDescription()))
            {
                updated = true;
            }
        }

        if (other->hasTags())
        {
            if (obj->setTags(other->getTags()))
            {
                updated = true;
            }
        }

        if (other->hasOrder())
        {
            if (obj->setOrder(other->getOrder()))
            {
                updated = true;
            }
        }

        if (other->hasParent())
        {
            // TODO: handle pending parent correctly...


            auto parent = other->getParent().lock();
            auto old_value = obj->parent.lock();

            if (parent)
            {
                if ((old_value && parent->getId() != old_value->getId())
                    || !old_value)
                {
                    // don't mark dirty
                    parent->addChildInternal(shared_from_this());
                    updated = true;
                }
            }
        }

        if (other->hasUserdata())
        {
            if (obj->setUserData(other->getUserdata()))
            {
                updated = true;
            }
        }

        if (other->hasUserid())
        {
            if (obj->setUserid(other->getUserid()))
            {
                updated = true;
            }
        }

        if (other->hasReadonly())
        {
            if (obj->setReadonly(other->getReadonly()))
            {
                updated = true;
            }
        }

        if (updated)
        {
            obj->callUpdatedCb();
        }
    }

    template <typename TD>
    void Parameter<TD>::dump() {

        std::cout << "parameter id: " << getId() << "\n";
        if (auto manager = obj->parameterManager.lock())
        {
            std::cout << "dirty: " << manager->isParameterDirty(shared_from_this()) << "\n";
        }
        else
        {
            std::cout << "no manager\n";
        }

        //
        getTypeDefinition().dump();

        //
        if (hasLabel())
        {
            std::cout << "label: " << getLabel() << "\n";
        }
        auto langs = getLabelLanguages();
        if (langs.size() < 0)
        {
            for (auto& code : langs)
            {
                std::cout << "label " << code << ": " << getLanguageLabel(code) << "\n";
            }
        }

        if (hasDescription())
        {
            std::cout << "description: " << getDescription() << "\n";
        }
        auto descs = getDescriptionLanguages();
        if (descs.size() < 0)
        {
            for (auto& code : descs)
            {
                std::cout << "description " << code << ": " << getDescriptionLanguage(code) << "\n";
            }
        }

        if (hasTags())
        {
            std::cout << "Tags: " << getTags() << "\n";
        }

        if (hasOrder())
        {
            std::cout << "order: " << getOrder() << "\n";
        }

        if (hasParent())
        {
            if (auto p = obj->parent.lock())
            {
                std::cout << "parent id: " << p->getId() << "\n";
            }
            else
            {
                std::cout << "parent id: -\n";
            }
        }

        if (hasUserdata())
        {
            std::cout << "has userdata\n";
        }

        if (hasUserid())
        {
            std::cout << "userid: " << getUserid() << "\n";
        }

        if (hasReadonly())
        {
            std::cout << "readonly: " << getReadonly() << "\n";
        }
    }

    template <typename TD>
    std::weak_ptr<GroupParameter>& Parameter<TD>::getParent() const
    {
        return obj->parent;
    }

    template <typename TD>
    void Parameter<TD>::removeFromParent()
    {
        // move to root-group
        if (auto manager = obj->parameterManager.lock())
        {
            GroupParameterPtr root_group = std::dynamic_pointer_cast<GroupParameter>(manager->getRootGroup());
            if (root_group)
            {
                root_group->addChild(shared_from_this());
            }
        }
    }

    template <typename TD>
    bool Parameter<TD>::setParentInternal(GroupParameterPtr parent)
    {
        auto old_value = obj->parent.lock();

        if (old_value)
        {
            if (parent->getId() == old_value->getId())
            {
                // parent already set
                return false;
            }

            // remove from parent...
            old_value->removeChild(shared_from_this());
        }

        m_waitForParent = false;

        obj->parent = parent;
        obj->parentChanged = true;

        // call listener
        for (auto listener : obj->m_parameterListener)
        {
            listener->onParentChanged(parent->getId(), old_value ? old_value->getId() : 0);
        }

        return true;
    }

    template <typename TD>
    void Parameter<TD>::setParent(GroupParameterPtr parent)
    {
        if (setParentInternal(parent))
        {
            setDirty();
        }
    }



    /*
    */
    class BangParameter : public Parameter<BangTypeDefinition>
    {
    public:
        BangParameter(const BangParameter& v) :
            Parameter<BangTypeDefinition>(v)
        {}

        BangParameter(int16_t id) :
            Parameter<BangTypeDefinition>(id)
        {}

        void update(const ParameterPtr& other) override
        {
            if (other.get() == this) {
                return;
            }

            if (other->getId() != getId()) {
                // different id - discard
                return;
            }

            if (!other->hasLabel() &&
                    !other->hasDescription() &&
                    !other->hasOrder() &&
                    !other->hasParent() &&
                    !other->hasReadonly() &&
                    !other->hasTags() &&
                    !other->hasUserdata() &&
                    !other->hasUserid())
            {
                // empty other
                m_cb();
            }
            else
            {
                Parameter::update(other);
            }
        }

        void bang() {
            setDirty();
        }

        void setFunction(const std::function<void()>&& cb) {
            m_cb = cb;
        }

    private:
        std::function<void()> m_cb;
    };

    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    /*
                                                                88                 ad88
          ,d                                                    88                d8"
          88                                                    88                88
        MM88MMM  8b       d8  8b,dPPYba,    ,adPPYba,   ,adPPYb,88   ,adPPYba,  MM88MMM
          88     `8b     d8'  88P'    "8a  a8P_____88  a8"    `Y88  a8P_____88    88
          88      `8b   d8'   88       d8  8PP"""""""  8b       88  8PP"""""""    88
          88,      `8b,d8'    88b,   ,a8"  "8b,   ,aa  "8a,   ,d88  "8b,   ,aa    88
          "Y888      Y88'     88`YbbdP"'    `"Ybbd8"'   `"8bbdP"Y8   `"Ybbd8"'    88
                     d8'      88
                    d8'       88
    */
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    typedef ValueParameter<bool, BooleanTypeDefinition, DATATYPE_BOOLEAN > BooleanParameter;
    typedef std::shared_ptr<BooleanParameter> BooleanParameterPtr;

    // number parameter
    typedef ValueParameter<int8_t, Int8TypeDefinition, DATATYPE_INT8 > Int8Parameter;
    typedef ValueParameter<uint8_t, UInt8TypeDefinition, DATATYPE_UINT8 > UInt8Parameter;
    typedef std::shared_ptr<Int8Parameter> Int8ParameterPtr;
    typedef std::shared_ptr<UInt8Parameter> UInt8ParameterPtr;

    typedef ValueParameter<int16_t, Int16TypeDefinition, DATATYPE_INT16 > Int16Parameter;
    typedef ValueParameter<uint16_t, UInt16TypeDefinition, DATATYPE_UINT16 > UInt16Parameter;
    typedef std::shared_ptr<Int16Parameter> Int16ParameterPtr;
    typedef std::shared_ptr<UInt16Parameter> UInt16ParameterPtr;

    typedef ValueParameter<int32_t, Int32TypeDefinition, DATATYPE_INT32 > Int32Parameter;
    typedef ValueParameter<uint32_t, UInt32TypeDefinition, DATATYPE_UINT32 > UInt32Parameter;
    typedef std::shared_ptr<Int32Parameter> Int32ParameterPtr;
    typedef std::shared_ptr<UInt32Parameter> UInt32ParameterPtr;

    typedef ValueParameter<int64_t, Int64TypeDefinition, DATATYPE_INT64 > Int64Parameter;
    typedef ValueParameter<uint64_t, UInt64TypeDefinition, DATATYPE_UINT64 > UInt64Parameter;
    typedef std::shared_ptr<Int64Parameter> Int64ParameterPtr;
    typedef std::shared_ptr<UInt64Parameter> UInt64ParameterPtr;

    typedef ValueParameter<float, Float32TypeDefinition, DATATYPE_FLOAT32 > Float32Parameter;
    typedef std::shared_ptr<Float32Parameter> Float32ParameterPtr;

    typedef ValueParameter<double, Float64TypeDefinition, DATATYPE_FLOAT64 > Float64Parameter;
    typedef std::shared_ptr<Float64Parameter> Float64ParameterPtr;


    // vector parameter
    // vector 2
    typedef ValueParameter<Vector2<int>, Vector2I32TypeDefinition, DATATYPE_VECTOR2I32 > Vector2I32Parameter;
    typedef std::shared_ptr<Vector2I32Parameter> Vector2I32ParameterPtr;

    typedef ValueParameter<Vector2<float>, Vector2F32TypeDefinition, DATATYPE_VECTOR2F32 > Vector2F32Parameter;
    typedef std::shared_ptr<Vector2F32Parameter> Vector2F32ParameterPtr;

    // vector 3
    typedef ValueParameter<Vector3<int>, Vector3I32TypeDefinition, DATATYPE_VECTOR3I32 > Vector3I32Parameter;
    typedef std::shared_ptr<Vector3I32Parameter> Vector3I32ParameterPtr;

    typedef ValueParameter<Vector3<float>, Vector3F32TypeDefinition, DATATYPE_VECTOR3F32 > Vector3F32Parameter;
    typedef std::shared_ptr<Vector3F32Parameter> Vector3F32ParameterPtr;

    // vector 4
    typedef ValueParameter<Vector4<int>, Vector4I32TypeDefinition, DATATYPE_VECTOR4I32 > Vector4I32Parameter;
    typedef std::shared_ptr<Vector4I32Parameter> Vector4I32ParameterPtr;

    typedef ValueParameter<Vector4<float>, Vector4F32TypeDefinition, DATATYPE_VECTOR4F32 > Vector4F32Parameter;
    typedef std::shared_ptr<Vector4F32Parameter> Vector4F32ParameterPtr;



    typedef ValueParameter<std::string, StringTypeDefinition, DATATYPE_STRING > StringParameter;
    typedef std::shared_ptr<StringParameter> StringParameterPtr;

    typedef ValueParameter<TinyString, EnumTypeDefinition, DATATYPE_ENUM > EnumParameter;
    typedef std::shared_ptr<EnumParameter> EnumParameterPtr;

    // color parameter
    typedef ValueParameter<Color, RGBTypeDefinition, DATATYPE_RGB > RGBParameter;
    typedef std::shared_ptr<RGBParameter> RGBParameterPtr;

    typedef ValueParameter<Color, RGBATypeDefinition, DATATYPE_RGBA > RGBAParameter;
    typedef std::shared_ptr<RGBAParameter> RGBAParameterPtr;

    // uri parameter
    typedef ValueParameter<std::string, UriTypeDefinition, DATATYPE_URI > URIParameter;
    typedef std::shared_ptr<URIParameter> URIParameterPtr;

    // ip parameter
    typedef ValueParameter<IPv4, IPv4TypeDefinition, DATATYPE_IPV4 > IPv4Parameter;
    typedef std::shared_ptr<IPv4Parameter> IPv4ParameterPtr;

    typedef ValueParameter<IPv6, IPv6TypeDefinition, DATATYPE_IPV6 > IPv6Parameter;
    typedef std::shared_ptr<IPv6Parameter> IPv6ParameterPtr;

    // non-value parameter
    typedef std::shared_ptr<BangParameter> BangParameterPtr;

    typedef Parameter<InvalidTypeDefinition> InvalidParameter;
    typedef std::shared_ptr<InvalidParameter> InvalidParameterPtr;


    //
    template<
        typename T,
        typename TD,
        datatype_t type_id
    >
    std::ostream& operator<<(std::ostream& out, ValueParameter<T, TD, type_id>& param);
}



#endif
