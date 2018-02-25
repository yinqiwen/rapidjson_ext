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

#include "json_object.h"
#include "json_c.h"
#include <stdio.h>

static void test1()
{
    rapidjson_ext::JsonObject obj;
    int ret = obj.ParseFile("./k.json");
    printf("###%d %s\n", ret, obj.LastError().c_str());
    printf("%s\n", obj["settings"]["analysis"]["filter"]["my_aboost_filter"]["type"].GetString());

    CJsonData *k = new_jsondata();
    k->ParseFile(k, "./k.json");

    CJsonData v1 = k->GetMember(k, "settings", "analysis", "filter", "my_aboost_filter", "type", NULL);
    printf("%s\n", v1.GetString(&v1, ""));
    v1.SetInt(&v1, 123);
    char* str = k->ToString(k);
    printf("%s\n", str);
    free(str);

    delete_jsondata(k);
}

static void test2()
{
    CJsonData *k = new_jsondata();
    k->ParseFile(k, "./t.json");

    CJsonData e0 = k->GetArrayElement(k, 0);
    CJsonData e1 = k->GetArrayElement(k, 1);
    CJsonData e2 = k->GetArrayElement(k, 2);
    e2.Swap(&e2, &e0);

    CJsonData *tmp = k->NewJson(k);
    CJsonData tmpv = tmp->GetMember(tmp, "test", NULL);
    tmpv.SetString(&tmpv, "zzzzzz");
    CJsonData e3 = k->AddArrayElement(k);
    swap_json(&e3, tmp);
    char *str = tmp->ToString(k);
    printf("####%s\n", str);
    free(str);

    delete_jsondata(k);
    delete_jsondata(tmp);
}

int main()
{
    test1();
    test2();
    return 0;
}
