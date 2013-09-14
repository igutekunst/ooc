#ifndef OBJECT_H
#define OBJECT_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#define MAGIC ((unsigned long ) 0xBD71472)
struct class_header{
    unsigned long magic; 
    size_t size;
    size_t  ( *get_size) (const void * _self);
    size_t  ( *get_len) (const void * _self);
    size_t  ( *play) (const void * _self);
    const void *  ( * __construct__ ) (const void const * _self, va_list args);
    void*   ( * __destruct__  ) (void * _self);
    void    (* print)  (const void *);
    void*   (* to_string)  (const void *);
    const char*   (* str)  (const void *);
    void*   (* to_String)  (const void *);
    const void*   (* append) (const void const* _self, const void const * _other);
    const void*   (* copy) (const void const * _self);
    void*   (* insert) (const void const * _self, 
                        const void const * _key, 
                        const void const * item);
    uint32_t (* hash) (const void const * _self);

};

const void * type(const void const * _self);
const void *  new (const void const * _class, ...);

void *  del (void * _object);

void    print   (const void * _object);
size_t  size    (const void * _object);
size_t  len     (const void * _object);
const char*   str     (const void * _object);
void*   to_String  (const void * _object);
const void*   copy    (const void const* _object);
const void*   append  (const void const* _object, const void const * _other);
void*   iter    (void * _object);
void *  play   (void * _object);
uint32_t hash (const void const * _self);


extern const struct class_header Class;

bool is_obj(const void const * _self, const char *message);
const struct class_header * get_class_header(const void const * _self);

#endif
