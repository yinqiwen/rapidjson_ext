/*
 * json_c.h
 *
 *  Created on: 2018Äê2ÔÂ24ÈÕ
 *      Author: qiyingwang
 */

#ifndef RAPPIDJSON_EXT_JSON_C_H_
#define RAPPIDJSON_EXT_JSON_C_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif
    struct CJsonData
    {
            void* data;
            int (*ParseString)(struct CJsonData*, const char* content);
            int (*ParseFile)(struct CJsonData*, const char* file);
            int64_t (*GetInt)(struct CJsonData*, int64_t default_val);
            double (*GetDouble)(struct CJsonData*, double default_val);
            const char* (*GetString)(struct CJsonData*, const char* default_val);
            struct CJsonData (*GetMember)(struct CJsonData*, const char* name, ...);
            struct CJsonData (*GetElement)(struct CJsonData*, int idx);
            int64_t (*Size)(struct CJsonData*);
    };

    CJsonData* new_jsondata();
    void delete_jsondata(CJsonData* v);

#ifdef __cplusplus
}
#endif

#endif /* JSON_C_H_ */
