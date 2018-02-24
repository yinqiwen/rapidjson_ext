/*
 * test.cpp
 *
 *  Created on: 2018Äê2ÔÂ24ÈÕ
 *      Author: qiyingwang
 */
#include "json_object.h"
#include "json_c.h"
#include <stdio.h>

int main()
{
    rapidjson_ext::JsonObject obj;
    int ret = obj.ParseFile("./k.json");
    printf("###%d %s\n", ret, obj.LastError().c_str());
    printf("%s\n", obj["settings"]["analysis"]["filter"]["my_aboost_filter"]["type"].GetString());

    CJsonData* k = new_jsondata();
    k->ParseFile(k, "./k.json");

    CJsonData v1 = k->GetMember(k, "settings", "analysis", "filter", "my_aboost_filter", "type", NULL);
    printf("%s\n", v1.GetString(&v1, ""));
    delete_jsondata(k);
    return 0;
}



