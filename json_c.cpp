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
#include "json_c.h"
#include "json_object.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void init_jsondata(CJsonData *v, rapidjson_ext::JsonObject *obj);

static int CJsonParseString(struct CJsonData *v, const char *content)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    return jv->ParseString(content);
}
static int CJsonParseFile(struct CJsonData *v, const char *file)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    return jv->ParseFile(file);
}

static int64_t CJsonGetInt(struct CJsonData *v, int64_t default_val)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    if (!jv->IsInt())
    {
        return default_val;
    }
    return jv->GetInt();
}
static double CJsonGetDouble(struct CJsonData *v, double default_val)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    if (!jv->IsDouble())
    {
        return default_val;
    }
    return jv->GetDouble();
}
static const char *CJsonGetString(struct CJsonData *v, const char *default_val)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    if (!jv->IsString())
    {
        return default_val;
    }
    return jv->GetString();
}
static int CJsonGetBool(struct CJsonData *v, int default_val)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    if (!jv->IsBool())
    {
        return default_val;
    }
    return (int)jv->GetBool();
}

static void CJsonSetDouble(struct CJsonData *v, double default_val)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    jv->SetDouble(default_val);
}

static void CJsonSetInt(struct CJsonData *v, int64_t default_val)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    jv->SetInt(default_val);
}
static void CJsonSetString(struct CJsonData *v, const char *default_val)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    jv->SetString(default_val);
}
static void CJsonSetBool(struct CJsonData *v, int default_val)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    jv->SetBool((bool)default_val);
}
struct CJsonData CJsonGetMember(struct CJsonData *v, const char *name, ...)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    struct CJsonData vv;
    va_list valist;
    va_start(valist, name);
    for (const char *path = name; path != NULL; path = va_arg(valist, const char *))
    {
        jv = &(*jv)[path];
    }
    va_end(valist);
    init_jsondata(&vv, jv);
    return vv;
}
struct CJsonData CJsonGetArrayElement(struct CJsonData *v, int idx)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    struct CJsonData vv;
    jv = &((*jv)[idx]);
    init_jsondata(&vv, jv);
    return vv;
}

static struct CJsonData CJsonAddArrayElement(struct CJsonData *v)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    struct CJsonData vv;
    rapidjson_ext::JsonObject &ele = jv->Add();
    init_jsondata(&vv, &ele);
    return vv;
}
static struct CJsonData* CJsonNewJson(struct CJsonData *v)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    CJsonData *nv = (CJsonData *)malloc(sizeof(CJsonData));
    init_jsondata(nv, new rapidjson_ext::JsonObject(jv->GetAllocator()));
    return nv;
}

void swap_json(struct CJsonData *a, struct CJsonData *b)
{
    rapidjson_ext::JsonObject *jv1 = (rapidjson_ext::JsonObject *)a->data;
    rapidjson_ext::JsonObject *jv2 = (rapidjson_ext::JsonObject *)b->data;
    jv1->Swap(*jv2);
}

static size_t CJsonSize(struct CJsonData *v)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    return jv->Size();
}
static char *CJsonToString(struct CJsonData *v)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    std::string str;
    jv->ToString(str);
    return strdup(str.c_str());
}

static void init_jsondata(CJsonData *v, rapidjson_ext::JsonObject *obj)
{
    v->data = obj;
    v->ParseString = CJsonParseString;
    v->ParseFile = CJsonParseFile;
    v->GetInt = CJsonGetInt;
    v->GetString = CJsonGetString;
    v->GetMember = CJsonGetMember;
    v->GetDouble = CJsonGetDouble;
    v->GetBool = CJsonGetBool;
    v->SetBool = CJsonSetBool;
    v->SetInt = CJsonSetInt;
    v->SetDouble = CJsonSetDouble;
    v->SetString = CJsonSetString;
    v->AddArrayElement = CJsonAddArrayElement;
    v->GetArrayElement = CJsonGetArrayElement;
    v->Size = CJsonSize;
    v->ToString = CJsonToString;
    v->Swap = swap_json;
    v->NewJson = CJsonNewJson;
}

CJsonData *new_jsondata()
{
    CJsonData *v = (CJsonData *)malloc(sizeof(CJsonData));
    init_jsondata(v, new rapidjson_ext::JsonObject);
    return v;
}

void delete_jsondata(CJsonData *v)
{
    rapidjson_ext::JsonObject *jv = (rapidjson_ext::JsonObject *)v->data;
    delete jv;
    free(v);
}
