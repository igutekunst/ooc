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
    void *  ( * __construct__ ) (void * _self, ...);
    void *  ( * __destruct__ ) (void * _self);
    void * (* print) ();

}Class;

void * new (void * _class, ...);

void * delete (void * _object);

#endif
