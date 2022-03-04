#ifndef _STUDENT_H
#define _STUDENT_H

#include "cJSON.h"

typedef struct student
{
    char name[16];
    int num;
    float temperature;
    int clocks;
}student;

int student_tocJSON(cJSON *obj, student *stdt);

#endif

