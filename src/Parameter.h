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

#ifndef RCP_PARAMETER_H
#define RCP_PARAMETER_H


#include <cstdint>
#include <memory>
#include <string>
#include <istream>
#include <set>
#include <unordered_map>

#include "IParameter.h"
#include "IGroupParameter.h"

#include "ParameterListener.h"
#include "ParameterManager.h"

#include "0.1.0/ParameterParser.h"
#include "0.1.0/types.h"

namespace rcp
{

template<typename> class Parameter;

template<typename TD>
using ParameterPtr = std::shared_ptr<Parameter<TD>>;


template<typename TD>
class Parameter
    : public virtual IParameter
    , public std::enable_shared_from_this<Parameter<TD>>
{
public:
    TD& typeDefinition()
    {
        return *m_typedefinition;
    }


    // listener

    void addUpdateListener(ParameterListener* listener)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (std::find(m_parameterListener.begin(), m_parameterListener.end(), listener) == m_parameterListener.end())
        {
            m_parameterListener.push_back(listener);
        }
    }

    void removeUpdateListener(ParameterListener* listener)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        auto it = std::find(m_parameterListener.begin(), m_parameterListener.end(), listener);
        if (it != m_parameterListener.end())
        {
            m_parameterListener.erase(it);
        }
    }



public:
    // IParameter

    IdType getId() const override
    {
        return m_id;
    }

    virtual uint8_t getDatatype() const override
    {
        return m_typedefinition->datatype();
    }


    // optional
    std::string getLabel() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return label;
    }

    void setLabel(const std::string& newLabel) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        std::string old_value = label;

        if (label != newLabel)
        {
            label = newLabel;
            m_hasLabel = true;
            labelChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_LABEL);

            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onLabelChanged(*this, old_value);
            }

            setDirty();
        }
    }
    bool hasLabel() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return m_hasLabel;
    }
    void clearLabel() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (m_hasLabel)
        {
            m_hasLabel = false;
            labelChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_LABEL);

            setDirty();
        }
    }

    // label languages
    std::vector<std::string> getLabelLanguages() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        std::vector<std::string> keys;
        for (auto it = languageLabel.begin(); it != languageLabel.end(); ++it)
        {
            keys.push_back(it->first);
        }

        return keys;
    }

    std::string getLanguageLabel(const std::string& code) const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        auto it = languageLabel.find(code);

        if (it != languageLabel.end())
        {
            return it->second;
        }

        return "";
    }

    void clearLanguageLabel() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (!languageLabel.empty())
        {
            languageLabel.clear();
            labelChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_LABEL);

            setDirty();
        }
    }

    void setLanguageLabel(const std::string& code, const std::string& label) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (code.size() != 3)
        {
            // ignore
            return;
        }

        std::string old_value = getLanguageLabel(code);
        if (label != old_value)
        {
            languageLabel[code] = label;
            labelChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_LABEL);

            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onLanguageLabelChanged(*this, code, old_value);
            }

            setDirty();
        }
    }

    void removeLanguageLabel(const std::string& code) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        auto it = languageLabel.find(code);

        if (it != languageLabel.end())
        {
            languageLabel.erase(it);

            labelChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_LABEL);

            setDirty();
        }
    }







    std::string getDescription() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return description;
    }
    void setDescription(const std::string& newDescription) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        auto old_value = getDescription();

        if (description != newDescription)
        {
            std::string old_value = description;

            description = newDescription;
            m_hasDescription = true;
            descriptionChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_DESCRIPTION);


            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onDescriptionChanged(*this, old_value);
            }

            setDirty();
        }
    }
    bool hasDescription() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return m_hasDescription;
    }
    void clearDescription() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (m_hasDescription)
        {
            m_hasDescription = false;
            descriptionChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_DESCRIPTION);

            setDirty();
        }
    }

    // description languages
    std::vector<std::string> getDescriptionLanguages() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        std::vector<std::string> keys;
        for (auto it = languageDescription.begin(); it != languageDescription.end(); ++it)
        {
            keys.push_back(it->first);
        }

        return keys;
    }

    std::string getDescriptionLanguage(const std::string& code) const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        auto it = languageDescription.find(code);
        if (it != languageDescription.end())
        {
            return it->second;
        }

        return "";
    }

    void clearDescriptionLanguage() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (!languageDescription.empty())
        {
            languageDescription.clear();
            descriptionChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_DESCRIPTION);

            setDirty();
        }
    }

    void setDescriptionLanguage(const std::string& code, const std::string& newDescription) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (code.size() != 3)
        {
            // ignore
            return;
        }

        std::string old_value = getDescriptionLanguage(code);

        if (newDescription != old_value)
        {
            languageDescription[code] = newDescription;
            descriptionChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_DESCRIPTION);

            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onLanguageDescriptionChanged(*this, code, old_value);
            }

            setDirty();
        }
    }

    void removeDescriptionLanguage(const std::string& code) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        auto it = languageDescription.find(code);
        if (it != languageDescription.end())
        {
            languageDescription.erase(it);

            descriptionChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_DESCRIPTION);

            setDirty();
        }
    }

    //----------------------
    // tags
    std::string getTags() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return tags.valueOr("");
    }
    void setTags(const std::string& newTags) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        std::string old_value = getTags();

        if (updateOption(tags, newTags, v010::PARAMETER_OPTIONS_TAGS))
        {
            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onTagsChanged(*this, old_value);
            }
        }
    }
    bool hasTags() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return tags.hasValue();
    }
    void clearTags() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (clearOption(tags, v010::PARAMETER_OPTIONS_TAGS))
        {
            // TODO: call listener?
        }

    }


    //----------------------
    // order
    int32_t getOrder() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        return order.valueOr(0);
    }
    void setOrder(const int32_t newOrder) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        int32_t old_value = getOrder();

        if (updateOption(order, newOrder, v010::PARAMETER_OPTIONS_ORDER))
        {
            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onOrderChanged(*this, old_value);
            }
        }
    }
    bool hasOrder() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return order.hasValue();
    }
    void clearOrder() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (clearOption(order, v010::PARAMETER_OPTIONS_ORDER))
        {
            // TODO: call listener?
        }
    }

    //----------------------
    // parent
    virtual std::weak_ptr<IGroupParameter> getParent() const override
    {
        return m_parent;
    }

    virtual IdType getParentId() const override
    {
        if (auto p = m_parent.lock())
        {
            return p->getId();
        }

        return 0; //??
    }

    bool hasParent() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return m_parent.lock() != nullptr;
    }

    void setParent(IGroupParameterPtr parent) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (setParentInternal(parent))
        {
            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_PARENTID);

            setDirty();
        }
    }

    virtual void removeFromParent() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        // move to root-group
        if (auto manager = m_manager.lock())
        {
            auto root_group = manager->getRootGroup();
            if (root_group)
            {
                root_group->addChild(std::enable_shared_from_this<Parameter<TD>>::shared_from_this());
            }
        }
        else
        {
            clearParentInternal();
        }
    }


    //----------------------
    // m_userdata
    std::vector<char> getUserdata() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return m_userdata;
    }

    void setUserdata(const std::vector<char>& userdata) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        auto old_value = getUserdata();

        m_userdata = userdata;
        userdataChanged = true;

        // NOTE: parameter-option ids stay the same between v010 and v10
        m_changed.insert(v010::PARAMETER_OPTIONS_USERDATA);

        // call listener
        for (auto listener : m_parameterListener)
        {
            listener->onUserdataChanged(*this, old_value);
        }

        setDirty();
    }
    bool hasUserdata() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return !m_userdata.empty();
    }
    void clearUserdata() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (!m_userdata.empty())
        {
            m_userdata.clear();
            userdataChanged = true;

            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(v010::PARAMETER_OPTIONS_USERDATA);

            setDirty();
        }
        else
        {
            userdataChanged = false;
        }
    }

    //----------------------
    // m_userid
    std::string getUserid() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        return m_userid.valueOr("");
    }
    void setUserid(const std::string& userid) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        std::string old_value = getUserid();

        if (updateOption(m_userid, userid, v010::PARAMETER_OPTIONS_USERID))
        {
            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onUseridChanged(*this, old_value);
            }
        }
    }
    bool hasUserid() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return m_userid.hasValue();
    }
    void clearUserid() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        auto old_value = m_userid.value();

        if (clearOption(m_userid, v010::PARAMETER_OPTIONS_USERID))
        {
            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onUseridChanged(*this, old_value);
            }
        }
    }

    //----------------------
    // readonly
    bool getReadonly() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        return readonly.valueOr(false);
    };
    void setReadonly(const bool newReadonly) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (updateOption(readonly, newReadonly, v010::PARAMETER_OPTIONS_READONLY))
        {
            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onReadonlyChanged(*this);
            }
        }
    }
    bool hasReadonly() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        return readonly.hasValue();
    }

    void clearReadonly() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        if (clearOption(readonly, v010::PARAMETER_OPTIONS_READONLY))
        {
            // call listener
            for (auto listener : m_parameterListener)
            {
                listener->onReadonlyChanged(*this);
            }
        }
    }







    virtual void parseTypeOptions(std::istream& is) override
    {
        // parse type options first
        m_typedefinition->parseOptions(is);
    }

    virtual void parseElementTypeOptions(std::istream& is) override
    {
        m_typedefinition->parseElementOptions(is);
    }


    virtual void writeTypeDefinition(Writer &out, bool all) override
    {
        // call writer??
        m_typedefinition->write(out, all);            
    }

    virtual void writeTypeMandatory(Writer &out) override
    {
        m_typedefinition->writeMandatory(out);
    }



    virtual void update(IParameterPtr other) override
    {
        if (other.get() == this)
        {
            return;
        }

        if (m_id != other->getId())
        {
            std::cerr << "update from wrong id" << std::endl;
            return;
        }

        auto typed_param = std::dynamic_pointer_cast<Parameter<TD>>(other);
        if (typed_param)
        {
            bool updated = updateInternal(typed_param);

            if (updated)
            {
                for (auto& fn : m_updatedCallbacks)
                {
                    fn.second();
                }
            }
        }
        else
        {
            std::cerr << "update from wrong type" << std::endl;
        }
    }

    void setDirty() override
    {
        // NOTE: setDirty is usually called from locked setters
        // no lock here
        if (auto manager = m_manager.lock())
        {
            if (manager->setParameterDirty(std::enable_shared_from_this<Parameter<TD>>::shared_from_this()))
            {
                // call listener
                for (auto listener : m_parameterListener)
                {
                    listener->onDirty(*this);
                }
            }
        }
    }

    std::recursive_mutex& mutex() const override
    {
        return m_mutex;
    }


    void setManager(ParameterManagerPtr manager) override
    {
        m_manager = manager;
    }

    virtual bool anyOptionChanged() override
    {
        return !m_changed.empty();
    }

    virtual void setAllUnchanged() override
    {
        labelChanged = false;
        descriptionChanged = false;
        tags.setUnchanged();
        order.setUnchanged();
        parentChanged = false;
        userdataChanged = false;
        m_userid.setUnchanged();

        m_typedefinition->setAllUnchanged();

        m_changed.clear();
    }

    virtual bool onlyValueChanged() override
    {
        return !m_typedefinition->isDirty()
               && m_changed.size() == 1
               && *m_changed.begin() == v010::PARAMETER_OPTIONS_VALUE;
    }


    // update callbacks

    void addUpdatedCb(void* obj, const std::function<void()>& fn) override
    {
        m_updatedCallbacks[obj] = fn;
    }

    void removeUpdatedCb(void* obj) override
    {
        if (m_updatedCallbacks.find(obj) != m_updatedCallbacks.end())
        {
            m_updatedCallbacks.erase(obj);
        }
    }

    void clearUpdatedCb() override
    {
        m_updatedCallbacks.clear();
    }


public:
    virtual bool setParentInternal(IGroupParameterPtr parent) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif

        auto old_value = m_parent.lock();

        if (old_value)
        {
            if (parent == old_value)
            {
                // parent already set
                return false;
            }

            // remove from parent...
            old_value->removeChild(std::enable_shared_from_this<Parameter<TD>>::shared_from_this());
        }
        else if (parent == nullptr)
        {
            return false;
        }

        m_parent = parent;
        parentChanged = true;

        // call listener
        for (auto listener : m_parameterListener)
        {
            listener->onParentChanged(*this, old_value ? old_value->getId() : 0);
        }

        return true;
    }

    virtual void clearParentInternal() override
    {
        auto old_value = m_parent.lock();

        if (!old_value)
        {
            parentChanged = false;
        }

        m_parent.reset();
        parentChanged = true;
    }



protected:
    Parameter(IdType id, const std::string& label = "")
        : IParameter(id, label)
        , m_typedefinition(new TD(this))
    {}

    ~Parameter()
    {
        if (m_typedefinition)
        {
            delete m_typedefinition;
        }
    }


    bool updateInternal(ParameterPtr<TD> other)
    {
        // update type definition
        m_typedefinition->update(*other->m_typedefinition);


        // update parameter
        bool updated = false;

        if (other->hasLabel())
        {
            // copy other language labels
            auto o_lang = other->getLabelLanguages();
            if (!o_lang.empty())
            {
                for (std::string& l : o_lang)
                {
                    std::string old_value = getLanguageLabel(l);
                    if (old_value != other->getLanguageLabel(l))
                    {
                        languageLabel[l] = label;

                        updated = true;

                        // call listener
                        for (auto listener : m_parameterListener)
                        {
                            listener->onLanguageLabelChanged(*this, l, old_value);
                        }
                    }
                }
            }

            // default label
            std::string old_value = getLabel();
            if (old_value != other->getLabel())
            {
                label = other->getLabel();
                m_hasLabel = true;

                updated = true;

                // call listener
                for (auto listener : m_parameterListener)
                {
                    listener->onLabelChanged(*this, old_value);
                }
            }

            labelChanged = false;

            // NOTE: parameter-option ids stay the same between v010 and v10
            this->m_changed.erase(v010::PARAMETER_OPTIONS_LABEL);
        }

        if (other->hasDescription())
        {
            // copy other language descriptions
            auto o_lang = other->getDescriptionLanguages();
            if (!o_lang.empty())
            {
                for (std::string& l : o_lang)
                {
                    std::string old_value = getDescriptionLanguage(l);
                    if (old_value != other->getDescriptionLanguage(l))
                    {
                        languageDescription[l] = other->getDescriptionLanguage(l);

                        updated = true;

                        // call listener
                        for (auto listener : m_parameterListener)
                        {
                            listener->onLanguageDescriptionChanged(*this, l, old_value);
                        }
                    }
                }
            }

            // default description
            auto old_value = getDescription();

            if (old_value != other->getDescription())
            {
                description = other->getDescription();
                m_hasDescription = true;

                updated = true;

                // call listener
                for (auto listener : m_parameterListener)
                {
                    listener->onDescriptionChanged(*this, old_value);
                }
            }

            descriptionChanged = false;

            // NOTE: parameter-option ids stay the same between v010 and v10
            this->m_changed.erase(v010::PARAMETER_OPTIONS_DESCRIPTION);
        }

        if (other->hasTags())
        {
            std::string old_value = getTags();

            if (old_value != other->getTags())
            {
                tags.setValue(other->getTags());

                updated = true;

                // call listener
                for (auto listener : m_parameterListener)
                {
                    listener->onTagsChanged(*this, old_value);
                }
            }

            tags.setUnchanged();

            // NOTE: parameter-option ids stay the same between v010 and v10
            this->m_changed.erase(v010::PARAMETER_OPTIONS_TAGS);
        }

        if (other->hasOrder())
        {
            auto old_value = getOrder();

            if (old_value != other->getOrder())
            {
                order.setValue(other->getOrder());

                updated = true;

                // call listener
                for (auto listener : m_parameterListener)
                {
                    listener->onOrderChanged(*this, old_value);
                }
            }

            order.setUnchanged();

            // NOTE: parameter-option ids stay the same between v010 and v10
            this->m_changed.erase(v010::PARAMETER_OPTIONS_ORDER);
        }

        if (other->hasParent())
        {
            // TODO: handle pending parent correctly...

            auto parent = other->getParent().lock();

            if (parent)
            {
                auto old_value = m_parent.lock();

                if (!old_value ||
                    (parent->getId() != old_value->getId()))
                {
                    // don't mark dirty
                    parent->addChildInternal(std::enable_shared_from_this<Parameter<TD>>::shared_from_this());
                    updated = true;
                }
            }

            parentChanged = false;

            // NOTE: parameter-option ids stay the same between v010 and v10
            this->m_changed.erase(v010::PARAMETER_OPTIONS_PARENTID);
        }

        if (other->hasUserdata())
        {
            auto old_value = getUserdata();

            // TODO: compare?
            if (true)
            {
                m_userdata = other->getUserdata();

                updated = true;

                // call listener
                for (auto listener : m_parameterListener)
                {
                    listener->onUserdataChanged(*this, old_value);
                }
            }

            userdataChanged = false;

            // NOTE: parameter-option ids stay the same between v010 and v10
            this->m_changed.erase(v010::PARAMETER_OPTIONS_USERDATA);
        }

        if (other->hasUserid())
        {
            std::string old_value = getUserid();

            if (old_value != other->getUserid())
            {
                m_userid.setValue(other->getUserid());

                updated = true;

                // call listener
                for (auto listener : m_parameterListener)
                {
                    listener->onUseridChanged(*this, old_value);
                }
            }

            m_userid.setUnchanged();

            // NOTE: parameter-option ids stay the same between v010 and v10
            this->m_changed.erase(v010::PARAMETER_OPTIONS_USERID);
        }

        if (other->hasReadonly())
        {
            if (getReadonly() != other->getReadonly())
            {
                readonly.setValue(other->getReadonly());

                updated = true;

                // call listener
                for (auto listener : m_parameterListener)
                {
                    listener->onReadonlyChanged(*this);
                }
            }

            readonly.setUnchanged();

            // NOTE: parameter-option ids stay the same between v010 and v10
            this->m_changed.erase(v010::PARAMETER_OPTIONS_READONLY);
        }

        return updated;
    }


protected:
    template<typename T>
    inline bool updateOption(Option<T>& o, T value, uint8_t id)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        o.setValue(value);
        if (o.changed())
        {
            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(id);

            this->setDirty();

            return true;
        }

        return false;
    }


    template<typename T>
    inline bool clearOption(Option<T>& o, uint8_t id)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_mutex);
#endif
        o.clearValue();
        if (o.changed())
        {
            // NOTE: parameter-option ids stay the same between v010 and v10
            m_changed.insert(id);

            this->setDirty();

            return true;
        }

        return false;
    }


protected:
    TD* m_typedefinition{nullptr};

    std::vector<ParameterListener*> m_parameterListener;

    mutable std::recursive_mutex m_mutex;

    // ParameterManager* m_manager{nullptr};
    std::weak_ptr<ParameterManager> m_manager;

    std::unordered_map<void*, std::function<void()>> m_updatedCallbacks;
};


}

#endif // PARAMETER_H
