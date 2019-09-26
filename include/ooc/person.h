#ifndef PERSON_H
#define PERSON_H
#include "object.h"
#include "string.h"

struct Person{
    struct class_header * class;
    size_t size;
    char * first;
    char * last;
    unsigned int age;
};
extern void * Person;

#endif
