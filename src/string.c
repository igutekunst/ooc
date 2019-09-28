#include <ooc/string.h>
#include "object_internal.h"
#include <string.h>
#include <stdlib.h>
#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U
#include <stdio.h>
#include <ooc/object.h>
#include "object_internal.h"

struct String{
    struct class_header * class;
    size_t size;
    size_t len;
    char * string_data;
};

struct StringClass{
    struct class_header class;
};

size_t String_get_len(const void * _self){
    struct String * self = (struct String *) _self;
    return self->len;
}


size_t String_get_size(const void * _self){
    struct String * self = (struct String *) _self;
    return self->size;
}


const void *__construct__String(const void *_self, size_t argc, va_list args) {
    struct String * self = (struct String *) _self;
    const char * data = va_arg(args, const char *);
    if (! data){
        free(self);
        return NULL;
    }

    size_t len = strlen(data);
    size_t size = sizeof(struct String)  + len + 1;
    self = (struct String *) realloc(self, size);
    self->class = String;
    void * space =  (void *) &self[1];
    self->string_data = strcpy(space, data);
    self->size = size;
    self->len = len;
    return self;
}


void print_String(void * _self) {
    struct String * self = (struct String *) _self;
    printf("%s", self->string_data);
}


const char * String_str(const void * _self){
    struct String * self = (struct String *) _self;
    return self->string_data;
}


const void * String_append(const void * _self, const void * _other){
    struct String * self = (struct String *) _self;
    const struct String * other = (const struct String *) _other;

    if (type(other) == String){

        char * new_char_data = (char *) malloc(len(self) + len(other) + 1 );

        strcpy(new_char_data, self->string_data);
        strcat(new_char_data, other->string_data);
        
        const struct String * new_string = new(String, new_char_data);
        return new_string;
        
        //
        self = realloc(self, size(self) + len(other));
        self->size += len(other);
        self->len += len(other);
        void * new_data = (void *) &new_string[1];

        // Since we are pointing new_string->string_data
        // at memory allocated at the end of self,
        // we may need to update the pointer if realloc 
        // moved the whole block, since self->string_data now points
        // to an address in the previous block over memory

        if (self->string_data != new_data) {
            strcpy(new_data, self->string_data);
            self->string_data = new_data;
        }

        self->string_data = strcat(self->string_data, other->string_data);
        return self;

    }else {
        printf("Cannot append some type to string\n") ;
        exit(1);
    }
}


bool String_equal(const void * _self, const void * _other){
    struct String * self = (struct String *) _self;
    const struct class_header * other = (const struct class_header *) _other;

    if (type(other) == String){
        return strcmp(c_str(self), c_str(other)) == 0;
    } else {
        printf("Cannot compare String to %s\n", clsname(other));
        exit(1);
    }
}


const void* String_copy(const void * _self){
    struct String * self = (struct String *) _self;
    return new(String, self->string_data);
}

uint32_t FNV32(char *s, size_t len) {
    // adapter from http://ctips.pbworks.com/w/page/7277591/FNV%20Hash
    uint32_t hash = FNV_OFFSET_32, i;
    for(i = 0; i < len; i++)
    {
        hash = hash ^ (s[i]); // xor next byte into the bottom of the hash
        hash = hash * FNV_PRIME_32; // Multiply by prime number found to work well
    }
    return hash;
}

uint64_t String_hash(const void * _self) {
    // See https://cp-algorithms.com/string/string-hashing.html
    // Consider https://github.com/haipome/fnv/blob/master/fnv.c
    struct String * self = (struct String *) _self;

    return FNV32(self->string_data, self->len);

    // TODO reduce collisions
    int64_t p = 53;
    int64_t m = 1e9 + 9;
    int64_t hash_value = 0;
    int64_t p_pow = 1;
    for (size_t i = 0; i < self->len; i++) {
        char c = self->string_data[i];
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}


struct StringClass string_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct String),
              .object_init = __construct__String,
              .get_size = String_get_size,
              .get_len = String_get_len,
              .c_str = String_str,
              .copy = String_copy,
              .append = String_append,
              .hash = String_hash,
              .equals = String_equal,
              .object_name = "String"
             }

};

void * String = &string_class;
