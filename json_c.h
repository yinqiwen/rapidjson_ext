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

#ifndef RAPPIDJSON_EXT_JSON_C_H_
#define RAPPIDJSON_EXT_JSON_C_H_

#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
struct CJsonData
{
    void *data;
    int (*ParseString)(struct CJsonData *, const char *content);
    int (*ParseFile)(struct CJsonData *, const char *file);
    int64_t (*GetInt)(struct CJsonData *, int64_t default_val);
    double (*GetDouble)(struct CJsonData *, double default_val);
    int (*GetBool)(struct CJsonData *, int default_val);
    const char *(*GetString)(struct CJsonData *, const char *default_val);
    void (*SetInt)(struct CJsonData *, int64_t default_val);
    void (*SetDouble)(struct CJsonData *, double default_val);
    void (*SetBool)(struct CJsonData *, int default_val);
    void (*SetString)(struct CJsonData *, const char *default_val);
    void (*Swap)(struct CJsonData *, struct CJsonData *);
    struct CJsonData (*AddArrayElement)(struct CJsonData *);
    struct CJsonData (*GetMember)(struct CJsonData *, const char *name, ...);
    struct CJsonData (*GetArrayElement)(struct CJsonData *, int idx);
    char* (*ToString)(struct CJsonData *);
    struct CJsonData* (*NewJson)(struct CJsonData *);

    size_t (*Size)(struct CJsonData *);
};

CJsonData *new_jsondata();
void delete_jsondata(CJsonData *v);
void swap_json(struct CJsonData *a, struct CJsonData *b);

#ifdef __cplusplus
}
#endif

#endif /* JSON_C_H_ */
