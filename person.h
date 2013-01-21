#ifndef PERSON_H
#define PERSON_H
#include <object.h>

struct Person{
    struct class_header class;
    char * first;
    char * last;
    unsigned int age;
};
extern void * Person;
void print(void * object);

#endif
