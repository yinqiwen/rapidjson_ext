/*
 * json_helper.h
 *
 *  Created on: 2016.10.17
 *      Author: qiyingwang
 */

#ifndef RAPPIDJSON_EXT_OBJECT_H_
#define RAPPIDJSON_EXT_OBJECT_H_

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <map>
#include <vector>
namespace rapidjson_ext
{
    class JsonObject
    {
        private:
            typedef std::map<std::string, JsonObject*> MemberTable;
            typedef std::vector<JsonObject*> MemberArray;
            MemberTable members_;
            MemberArray array_;
            rapidjson::Value* value_;
            rapidjson::Document::AllocatorType* allocator_;
            JsonObject* parent_;
            std::string name_;
            bool allocator_created_;
            bool value_created_;
            std::string err;
            bool HasMember();
            void BuildJsonObjectTree();
            bool Valid() const
            {
                return value_ != NULL;
            }
            JsonObject(const JsonObject& other)
            {
            }
            JsonObject& operator=(const JsonObject& other)
            {
            }
            void BuildFromValue(rapidjson::Value* v);
        public:
            class JsonReadIterator
            {
                private:
                    MemberTable::const_iterator iter;
                    const JsonObject* value_;
                    JsonReadIterator(const JsonObject* v);
                    friend class JsonObject;
                public:
                    bool NextMember(std::string& name, JsonObject*& value);
            };
            JsonObject(rapidjson::Document::AllocatorType* allocator = NULL);
            JsonObject(const rapidjson::Value* val);
            JsonObject(rapidjson::Value* val, rapidjson::Document::AllocatorType* allocator);
            int ParseString(const std::string& content);
            int ParseFile(const std::string& file);
            JsonObject& operator[](const std::string& name);
            JsonObject& operator[](int i);
            void SetInt(int64_t v);
            void SetBool(bool v);
            void SetString(const std::string& v);
            void SetDouble(double v);
            void SetObject();
            int64_t GetInt();
            double GetDouble();
            const char* GetString();
            JsonObject& Add();
            void Add(const std::string& str);
            void Add(int64_t v);
            void Add(double v);
            const std::string& ToString(std::string& str);
            void Clear();
            int64_t Size() const;
            JsonReadIterator MemberIterator();
            bool IsObject() const;
            bool IsArray() const;
            const std::string& LastError() const
            {
                return err;
            }
            ~JsonObject();
    };



}

#endif /* JSON_HELPER_H_ */
