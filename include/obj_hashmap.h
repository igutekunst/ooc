#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <object.h>
#include <string.h>
#include <stdint.h>

struct HashMapClass{
    struct class_header class;
};
struct HashMap{
    struct class_header * class;
    size_t size;
    size_t len;
    char * data;
};


struct HashItem{
    
};
extern void * HashMap;

#endif
