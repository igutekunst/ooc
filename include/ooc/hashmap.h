#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "object.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>


struct HashMapClass{
    struct class_header class;
};

struct HashMapClass_iter{
    struct class_header class;
};

struct HashItem {
    uint64_t key;
    const void * value;
    struct HashItem * next;
    bool present;
};
typedef struct HashItem HashItem;

struct HashMap{
    struct class_header * class;
    size_t size;
    size_t len;
    size_t m;
    size_t M;
    size_t hwm;
    size_t lwm;
    uint64_t a;
    uint64_t b;
    bool debug;
    struct HashItem * items;

};

struct HashMap_iter{
    struct class_header * class;
    struct HashItem * current_item;
    int index;
    struct HashMap * hash_map;
    int position;
};

extern void * HashMap;
extern void * HashMap_iter;
#define ODD 1
#define EVEN 0
// Hash Paramters. 
// See https://en.wikipedia.org/wiki/Universal_hashing
// For explanations of parameters
#define W   32
#define HASH_TABLE_DEFAULT_LEN 4
#define MAX_A  4294967295
#define HWM_FRACTION 0.5



#endif
