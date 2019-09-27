#ifndef OBJ_INT_H
#define OBJ_INT_H

#include "object.h"
#include <string.h>
#include <stdint.h>
#define MAX_STR_SIZE 20
struct Int{
    struct class_header * class;
    size_t size;
    int value;
    char str_value[MAX_STR_SIZE];
};
extern void * Int;

#endif
