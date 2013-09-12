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
    size_t ( *get_size) (void * _self);
    size_t ( *get_len) (void * _self);
    void *  ( * __construct__ ) (void * _self, va_list args);
    void*  ( * __destruct__  ) (void * _self);
    void    (* print)  (void *);
    char*   (* to_string)  (void *);
    void*   (* to_String)  (void *);
    void*   (* append) (void * _self, va_list args);

};

void * type(void * _self);
void *  new (void * _class, ...);

void *  delete (void * _object);

void    print   (void * _object);
size_t  size    (void * _object);
size_t  len     (void * _object);
char*   str     (void * _object);
void*   to_String  (void * _object);
void*   copy    (void * _object);
void*   append  (void * _object, void* _other);
void*   iter    (void * _object);
void *  index   (void * _object);


extern const struct class_header Class;

#endif
