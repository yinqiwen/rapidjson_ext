/*
 *Copyright (c) 2017-2017, yinqiwen <yinqiwen@gmail.com>
 *All rights reserved.
 *
 *Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Any name of this project nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RAPPIDJSON_EXT_OBJECT_H_
#define RAPPIDJSON_EXT_OBJECT_H_

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <map>
#include <vector>
#include <string>
namespace rapidjson_ext
{
class JsonObject
{
  private:
    typedef std::map<std::string, JsonObject *> MemberTable;
    typedef std::vector<JsonObject *> MemberArray;
    MemberTable members_;
    MemberArray array_;
    rapidjson::Value *value_;
    rapidjson::Document::AllocatorType *allocator_;
    JsonObject *parent_;
    std::string name_;
    bool allocator_created_;
    bool value_created_;
    std::string err;
    bool HasMember();
    bool Valid() const
    {
        return value_ != NULL;
    }
    JsonObject(const JsonObject &other)
    {
    }
    JsonObject &operator=(const JsonObject &other)
    {
        return *this;
    }
    void BuildFromValue(rapidjson::Value *v);

  public:
    class JsonReadIterator
    {
      private:
        MemberTable::const_iterator iter;
        const JsonObject *value_;
        JsonReadIterator(const JsonObject *v);
        friend class JsonObject;

      public:
        bool NextMember(std::string &name, JsonObject *&value);
    };
    JsonObject(rapidjson::Document::AllocatorType *allocator = NULL);
    JsonObject(const rapidjson::Value *val);
    JsonObject(rapidjson::Value *val, rapidjson::Document::AllocatorType *allocator);
    int ParseString(const std::string &content);
    int ParseFile(const std::string &file);
    JsonObject &operator[](const std::string &name);
    JsonObject &operator[](int i);
    void SetInt(int64_t v);
    void SetBool(bool v);
    void SetString(const std::string &v);
    void SetDouble(double v);
    void SetObject();
    void Swap(JsonObject& v);
    int64_t GetInt();
    double GetDouble();
    const char *GetString();
    bool GetBool();
    JsonObject &Add();
    void Add(const std::string &str);
    void Add(int64_t v);
    void Add(double v);
    const std::string &ToString(std::string &str);
    void Clear();
    int64_t Size() const;
    JsonReadIterator MemberIterator();
    bool IsObject() const;
    bool IsArray() const;
    bool IsInt() const;
    bool IsBool() const;
    bool IsString() const;
    bool IsDouble() const;
    const std::string &LastError() const
    {
        return err;
    }
    rapidjson::Document::AllocatorType * GetAllocator()
    {
        return allocator_;
    }
    ~JsonObject();
};
}

#endif /* JSON_HELPER_H_ */
