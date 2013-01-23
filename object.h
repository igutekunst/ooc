#ifndef OBJECT_H
#define OBJECT_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#define MAGIC ((unsigned long ) 0xBD714727255)
struct class_header{
    unsigned long magic; 
    size_t size;
    void *  ( * __construct__ ) (void * _self, va_list args);
    void *  ( * __destruct__ ) (void * _self);
    void  (* print) (void *);

};

void * new (void * _class, ...);

void * delete (void * _object);

void  print(void * _object);


extern const struct class_header Class;

#endif
