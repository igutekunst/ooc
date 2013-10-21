#ifndef OBJ_INT_H
#define OBJ_INT_H

#include "object.h"
#include <string.h>
#include <stdint.h>
struct IntClass{
    struct class_header class;
};
struct Int{
    struct class_header * class;
    size_t size;
    uint32_t value;
    char * str_value;
};
extern void * Int;

#endif
