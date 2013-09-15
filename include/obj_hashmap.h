#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <object.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


struct HashMapClass{
    struct class_header class;
};
typedef struct HashItem {
    uint32_t key;
    void * item;    
    struct HashItem * next;
};

struct HashMap{
    struct class_header * class;
    size_t size;
    size_t len;
    size_t m;
    size_t M;
    uint32_t a;
    uint32_t b;
    struct HashItem * items

};


extern void * HashMap;
#define ODD 1
#define EVEN 0
// Hash Paramters. 
// See https://en.wikipedia.org/wiki/Universal_hashing
// For explanations of parameters
#define W   32
#define HASH_TABLE_DEFAULT_LEN 32
#define MAX_A  4294967295



#endif
