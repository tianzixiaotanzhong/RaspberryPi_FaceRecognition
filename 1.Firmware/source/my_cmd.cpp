#include <iostream>
#include <string.h>
using namespace std;

int my_add_cmd_parse(int argc, const char** argv) {

}

int my_delete_cmd_parse(int argc, const char** argv) {
    
}

int my_show_cmd_parse(int argc, const char** argv) {
    
}

int my_edit_cmd_parse(int argc, const char** argv) {
    
}

int my_cmd_parse(int argc, const char** argv) {
    if (argc <= 0) {
        return -1;
    }
    if (!strcmp("add", argv[0])) {
        my_add_cmd_parse(argc, argv);
    }
    else if (!strcmp("delete", argv[0])) {
        my_delete_cmd_parse(argc, argv);
    }
    else if (!strcmp("show", argv[0])) {
        my_show_cmd_parse(argc, argv);
    }
    else if (!strcmp("edit", argv[0])) {
        my_edit_cmd_parse(argc, argv);
    }
    return 0;
}