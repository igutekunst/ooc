#ifndef OOC_STRING_H
#define OOC_STRING_H

#include "object.h"
#include <string.h>

struct StringClass{
    struct class_header class;
};

struct String{
    struct class_header * class;
    size_t size;
    size_t len;
    char * string_data;
};

extern void * String;

#endif
