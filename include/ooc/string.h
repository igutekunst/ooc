#ifndef OOC_STRING_H
#define OOC_STRING_H

#include "object.h"
#include <string.h>


struct String{
    struct class_header * class;
    size_t size;
    size_t len;
    char * string_data;
};

extern void * String;

#endif
