/*
 * json_c.cpp
 *
 *  Created on: 2018Äê2ÔÂ24ÈÕ
 *      Author: qiyingwang
 */
#include "json_c.h"
#include "json_object.h"
#include <stdarg.h>
#include <stdio.h>

static int CJsonParseString(struct CJsonData* v, const char* content)
{
    rapidjson_ext::JsonObject* jv = (rapidjson_ext::JsonObject*) v->data;
    return jv->ParseString(content);
}
static int CJsonParseFile(struct CJsonData* v, const char* file)
{
    rapidjson_ext::JsonObject* jv = (rapidjson_ext::JsonObject*) v->data;
    return jv->ParseFile(file);
}

static int64_t CJsonGetInt(struct CJsonData* v, int64_t default_val)
{
    rapidjson_ext::JsonObject* jv = (rapidjson_ext::JsonObject*) v->data;
    return jv->GetInt();
}
static const char* CJsonGetString(struct CJsonData* v, const char* default_val)
{
    rapidjson_ext::JsonObject* jv = (rapidjson_ext::JsonObject*) v->data;
    printf("####CJsonGetString\n");
    return jv->GetString();
}
struct CJsonData CJsonGetMember(struct CJsonData* v, const char* name, ...)
{
        rapidjson_ext::JsonObject* jv = (rapidjson_ext::JsonObject*) v->data;
        struct CJsonData vv;
        va_list valist;
        va_start(valist, name);
        printf("####here\n");
        for (const char* path = name; path != NULL; path = va_arg(valist, const char*)) {
            printf("####here%s\n", path);
            jv = &(*jv)[path];
        }
        va_end(valist);
        vv.data = jv;
        vv.ParseString = CJsonParseString;
        vv.ParseFile = CJsonParseFile;
        vv.GetInt = CJsonGetInt;
        vv.GetString = CJsonGetString;
        vv.GetMember = CJsonGetMember;
        printf("####here end\n");
        return vv;
}

CJsonData* new_jsondata()
{
    CJsonData* v = (CJsonData*) malloc(sizeof(CJsonData));
    v->data = new rapidjson_ext::JsonObject;
    v->ParseString = CJsonParseString;
    v->ParseFile = CJsonParseFile;
    v->GetInt = CJsonGetInt;
    v->GetString = CJsonGetString;
    v->GetMember = CJsonGetMember;
    return v;
}

void delete_jsondata(CJsonData* v)
{
    rapidjson_ext::JsonObject* jv = (rapidjson_ext::JsonObject*) v->data;
    delete jv;
    free(v);
}

