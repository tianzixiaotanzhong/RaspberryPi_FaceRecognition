#include <cJSON.h>
#include "manage.h"
#include "vector"
using namespace std;

int manage::add_user_info(int label, string name) {
    this->label = label;
    this->name = name;
    return 0;
}

int manage::record_temperature(int temp) {
    this->temperature = temp;
    return 0;
}