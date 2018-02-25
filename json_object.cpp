/*
 * json_helper.cpp
 *
 *  Created on: 2016.10.17
 *      Author: qiyingwang
 */
#include "json_object.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>

namespace rapidjson_ext
{
static JsonObject kempty(NULL, NULL);
static std::string kemptystr = "";

static inline void ltrim_string(std::string &s)
{
    int fisrt_non_space = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (!std::isspace(s[i]))
        {
            fisrt_non_space = i;
            break;
        }
    }
    s = s.substr(fisrt_non_space);
}
JsonObject::JsonObject(rapidjson::Document::AllocatorType *allocator)
    : value_(NULL), allocator_(allocator), parent_(NULL), allocator_created_(false), value_created_(true)
{
    if (NULL == allocator)
    {
        allocator_ = new rapidjson::Document::AllocatorType;
        allocator_created_ = true;
    }
    value_ = new rapidjson::Value;
}

JsonObject::JsonObject(rapidjson::Value *val, rapidjson::Document::AllocatorType *allocator)
    : value_(val), allocator_(allocator), parent_(NULL), allocator_created_(false), value_created_(false)
{
    if (NULL != val && NULL == allocator)
    {
        allocator_ = new rapidjson::Document::AllocatorType;
        allocator_created_ = true;
    }
    BuildFromValue(val);
}
void JsonObject::BuildFromValue(rapidjson::Value *val)
{
    if (NULL != val)
    {
        if (val->IsArray())
        {
            for (int i = 0; i < value_->Size(); i++)
            {
                rapidjson::Value *ele = &(*value_)[i];
                JsonObject *jv = new JsonObject(ele, allocator_);
                array_.push_back(jv);
            }
        }
        else if (val->IsObject())
        {
            rapidjson::Value::MemberIterator it = value_->MemberBegin();
            while (it != value_->MemberEnd())
            {
                JsonObject *jv = new JsonObject(&it->value, allocator_);
                std::string name = it->name.GetString();
                members_[name] = jv;
                it++;
            }
        }
    }
}
bool JsonObject::HasMember()
{
    return !members_.empty() || !array_.empty();
}
void JsonObject::SetInt(int64_t v)
{
    value_->SetInt64(v);
}
void JsonObject::SetBool(bool v)
{
    value_->SetBool(v);
}
void JsonObject::SetString(const std::string &v)
{
    value_->SetString(v.data(), v.size(), *allocator_);
}
void JsonObject::SetDouble(double v)
{
    if (NULL == value_)
    {
        return;
    }
    value_->Clear();
    value_->SetDouble(v);
}
void JsonObject::SetObject()
{
    if (NULL == value_)
    {
        return;
    }
    value_->SetObject();
}
void JsonObject::Swap(JsonObject &v)
{
    if (NULL == value_)
    {
        return;
    }
    value_->Swap(*v.value_);
}
int64_t JsonObject::GetInt()
{
    if (NULL == value_)
    {
        return 0;
    }
    return value_->GetInt64();
}
double JsonObject::GetDouble()
{
    if (NULL == value_)
    {
        return 0;
    }
    return value_->GetDouble();
}
bool JsonObject::GetBool()
{
    if (NULL == value_ || !value_->IsBool())
    {
        return false;
    }
    return value_->GetBool();
}
const char *JsonObject::GetString()
{
    if (NULL == value_)
    {
        return "";
    }
    if (!value_->IsString())
    {
        return "";
    }
    return value_->GetString();
}
JsonObject &JsonObject::operator[](const std::string &name)
{
    if (NULL == value_)
    {
        return kempty;
    }
    if (!value_->IsObject())
    {
        value_->SetObject();
    }
    JsonObject *v = NULL;
    MemberTable::iterator found = members_.find(name);
    if (found == members_.end())
    {
        rapidjson::Value tmp;
        rapidjson::Value nv(name.data(), name.size(), *allocator_);
        //nv.SetString(name.data(), name.size());
        //rapidjson::Value::StringRefType nv(name.data(), name.size());
        value_->AddMember(nv, tmp, *allocator_);
        rapidjson::Value::MemberIterator it = value_->FindMember(name.c_str());
        v = new JsonObject(&it->value, allocator_);
        v->parent_ = this;
        v->name_ = name;
        members_[name] = v;
    }
    else
    {
        v = found->second;
    }
    return *v;
}
JsonObject &JsonObject::Add()
{
    if (NULL == value_)
    {
        return kempty;
    }
    if (!value_->IsArray())
    {
        value_->SetArray();
    }
    rapidjson::Value tmp;
    value_->PushBack(tmp, *allocator_);
    rapidjson::Value &member = (*value_)[value_->Size() - 1];
    JsonObject *v = new JsonObject(&member, allocator_);
    v->parent_ = this;
    array_.push_back(v);
    return *v;
}
void JsonObject::Add(const std::string &str)
{
    if (NULL == value_)
    {
        return;
    }
    JsonObject &v = Add();
    v.SetString(str);
}
void JsonObject::Add(int64_t v)
{
    if (NULL == value_)
    {
        return;
    }
    JsonObject &val = Add();
    val.SetInt(v);
}
void JsonObject::Add(double v)
{
    if (NULL == value_)
    {
        return;
    }
    JsonObject &val = Add();
    val.SetDouble(v);
}
JsonObject &JsonObject::operator[](int i)
{
    if (NULL == value_)
    {
        return kempty;
    }
    if (i >= array_.size())
    {
        return kempty;
    }
    return *(array_[i]);
}

const std::string &JsonObject::ToString(std::string &str)
{
    if (NULL == value_)
    {
        return kemptystr;
    }
    str.clear();
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    value_->Accept(writer);
    str = buffer.GetString();
    return str;
}
void JsonObject::Clear()
{
    MemberTable::iterator it = members_.begin();
    while (it != members_.end())
    {
        delete it->second;
        it++;
    }
    members_.clear();
    for (int i = 0; i < (int)array_.size(); i++)
    {
        delete array_[i];
    }
    array_.clear();
}
int64_t JsonObject::Size() const
{
    if (array_.size() > 0)
    {
        return array_.size();
    }
    return members_.size();
}
bool JsonObject::IsObject() const
{
    if (NULL == value_)
    {
        return false;
    }
    return value_->IsObject();
}
bool JsonObject::IsArray() const
{
    if (NULL == value_)
    {
        return false;
    }
    return value_->IsArray();
}
bool JsonObject::IsInt() const
{
    if (NULL == value_)
    {
        return false;
    }
    return value_->IsInt();
}
bool JsonObject::IsBool() const
{
    if (NULL == value_)
    {
        return false;
    }
    return value_->IsBool();
}
bool JsonObject::IsString() const
{
    if (NULL == value_)
    {
        return false;
    }
    return value_->IsString();
}
bool JsonObject::IsDouble() const
{
    if (NULL == value_)
    {
        return false;
    }
    return value_->IsDouble();
}

int JsonObject::ParseString(const std::string &content)
{
    rapidjson::Document *doc = new rapidjson::Document(allocator_);
    rapidjson::ParseResult bParseok = doc->Parse(content.c_str(), content.size());
    if (bParseok.IsError())
    {
        err = "Invalid json to parse:" + content;
        delete doc;
        return -1;
    }
    if(value_created_)
    {
        delete value_;
    }
    value_ = doc;
    value_created_ = true;
    BuildFromValue(value_);
    return 0;
}
int JsonObject::ParseFile(const std::string &file)
{
    std::ifstream f(file.c_str());
    if (!f.is_open())
    {
        err = "Open json file failed.";
        return -1;
    }
    std::string content;
    std::string line;
    while (std::getline(f, line))
    {
        ltrim_string(line);
        if (line.find("//") == 0 || line.find("#") == 0)
        {
            continue;
        }
        content.append(line);
    }
    return ParseString(content);
}
JsonObject::~JsonObject()
{
    Clear();
    if (allocator_created_)
    {
        delete allocator_;
    }
    if (value_created_)
    {
        delete value_;
    }
}

JsonObject::JsonReadIterator JsonObject::MemberIterator()
{
    return JsonReadIterator(this);
}
JsonObject::JsonReadIterator::JsonReadIterator(const JsonObject *v)
    : value_(v)
{
    if (NULL != value_)
    {
        if (!value_->IsObject() || value_->Size() == 0)
        {
            value_ = NULL;
        }
        else
        {
            iter = value_->members_.begin();
        }
    }
}
bool JsonObject::JsonReadIterator::NextMember(std::string &name, JsonObject *&value)
{
    if (NULL == value_)
    {
        return false;
    }
    if (iter != value_->members_.end())
    {
        name = iter->first;
        value = (iter->second);
        iter++;
        return true;
    }
    return false;
}
}
