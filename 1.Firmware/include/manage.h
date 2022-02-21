#ifndef _MANAGE_H
#define _MANAGE_H

#include <string>

class manage
{
private:
    int label;
    string name;
    int temperature;
public:
    manage(int label, string name);
    ~manage();
};

manage::manage(int label, string name)
{
    this->label = label;
    this->name = name;
}

manage::~manage()
{
}


#endif
