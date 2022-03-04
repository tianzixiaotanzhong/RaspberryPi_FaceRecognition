#include "student.h"
#include "cJSON.h"

int student_tocJSON(cJSON *obj, student *stdt) {
    //cJSON *objs;
    //cJSON *obj;
    cJSON *num;
    cJSON *name;
    cJSON *temperature;
    cJSON *CardTime;
    //objs = cJSON_CreateArray();
    //obj = cJSON_CreateObject();
    num = cJSON_CreateNumber(stdt->num);
    if (NULL == num) {
        return -1;
    }
    name = cJSON_CreateString(stdt->name);
    if (NULL == name) {
        return -1;
    }
    temperature = cJSON_CreateNumber(stdt->temperature);
    if (NULL == temperature) {
        return -1;
    }
    CardTime = cJSON_CreateNumber(stdt->clocks);
    if (NULL == CardTime) {
        return -1;
    }
    cJSON_AddItemToObject(obj, "num", num);
    cJSON_AddItemToObject(obj, "name", name);
    cJSON_AddItemToObject(obj, "BodyTemperature", temperature);
    cJSON_AddItemToObject(obj, "CardTime", CardTime);

    return 0;
}
