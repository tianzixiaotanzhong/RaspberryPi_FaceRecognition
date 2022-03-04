#include <vector>
#include "alilink.h"
#include "cJSON.h"
#include "student.h"

using namespace std;

extern vector<student> stdts;
void *alilink_entry(void *arg) {
    cJSON *cjson_stdt;
    cJSON *cjson_stdts;
    cjson_stdt = cJSON_CreateObject()
    cjson_stdts = cJSON_CreateArray();
    for (auto stdt: stdts) {
        student_tocJSON(cjson_stdt, stdt);
        cJSON_AddItemToArray(cjson_stdts, cjson_stdt);
    }
}


