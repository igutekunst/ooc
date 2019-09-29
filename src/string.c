#include <ooc/string.h>
#include "object_internal.h"
#include <string.h>
#include <stdlib.h>

#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U

#include <stdio.h>
#include <ooc/object.h>
#include <ooc/list.h>
#include "object_internal.h"

struct String {
    struct class_header* class;
    size_t size;
    size_t len;
    char* string_data;
};

struct StringClass {
    struct class_header class;
};

size_t String_get_len(const void* _self) {
    struct String* self = (struct String*) _self;
    return self->len;
}


size_t String_get_size(const void* _self) {
    struct String* self = (struct String*) _self;
    return self->size;
}


const void* __construct__String(const void* _self, size_t argc, va_list args) {
    (void) argc;
    struct String* self = (struct String*) _self;
    const char* data = va_arg(args, const char *);
    if (!data) {
        free(self);
        return NULL;
    }

    // TODO allow string to be created using data without copying

    size_t len = strlen(data);
    size_t size = sizeof(struct String) + len + 1;
    self = (struct String*) realloc(self, size);
    self->class = String;
    void* space = (void*) &self[1];
    self->string_data = strcpy(space, data);
    self->size = size;
    self->len = len;
    return self;
}


void print_String(void* _self) {
    struct String* self = (struct String*) _self;
    printf("%s", self->string_data);
}


const char* String_str(const void* _self) {
    struct String* self = (struct String*) _self;
    return self->string_data;
}


const void* String_append(const void* _self, const void* _other) {
    struct String* self = (struct String*) _self;
    const struct String* other = (const struct String*) _other;

    if (type(other) == String) {

        char* new_char_data = (char*) malloc(len(self) + len(other) + 1);

        strcpy(new_char_data, self->string_data);
        strcat(new_char_data, other->string_data);

        const struct String* new_string = new(String, new_char_data);
        return new_string;

    } else {
        printf("Cannot append some type to string\n");
        exit(1);
    }
}


bool String_equal(const void* _self, const void* _other) {
    struct String* self = (struct String*) _self;
    const struct class_header* other = (const struct class_header*) _other;

    if (type(other) == String) {
        return strcmp(c_str(self), c_str(other)) == 0;
    } else {
        printf("Cannot compare String to %s\n", class_name(other));
        exit(1);
    }
}


const void* String_copy(const void* _self) {
    struct String* self = (struct String*) _self;
    return new(String, self->string_data);
}

const void* String_split(const void* _self, const void* _sep) {
    struct String* self = (struct String*) _self;
    //struct String* sep = (struct String*) _sep;

    if (_sep != NULL) {
        fprintf(stderr, "String_split only support splitting on space for now\n");
        exit(EXIT_FAILURE);
    }

    // make a copy in case strtok messes with it
    // TODO find out if it does
    const struct String* self_copy = copy(_self);
    const char* str = c_str(self_copy);

    const struct List* list = new(List);
    char* token;
    char* sep = " ";

    token = strtok((char*) str, sep);

    while( token != NULL ) {

        append(list, new(String, token));
        token = strtok(NULL, sep);
    }

    del(self_copy);
    return list;

}

const void* String_slice (const void* _self, ssize_t start, ssize_t end) {
    struct String* self = (struct String*) _self;
    const struct String* self_copy = copy(_self);

    if (len(_self) == 0) {
        return new(String, "");
    }

    // Note, you usually shouldn't modify the internals
    // of a String object
    char* c_copy = (char*) c_str(self_copy);

    if (start < 0) {
        start = len(_self) + start;
    }

    if (end < 0) {
        end = len(_self) + end;
    }

    const struct String* out = NULL;
    if (start >= len(_self)) {
       start = len(_self);
    }


    if (end > len(_self)) {
        end = len(_self);
    }

    c_copy[end] = '\0';
    out = new(String, &c_copy[start]);

    del(self_copy);
    return out;
}

uint32_t FNV32(const char* s, size_t len) {
    // adapter from http://ctips.pbworks.com/w/page/7277591/FNV%20Hash
    uint32_t hash = FNV_OFFSET_32, i;
    for (i = 0; i < len; i++) {
        hash = hash ^ (s[i]); // xor next byte into the bottom of the hash
        hash = hash * FNV_PRIME_32; // Multiply by prime number found to work well
    }
    return hash;
}

uint64_t String_hash(const void* _self) {
    // See https://cp-algorithms.com/string/string-hashing.html
    // Consider https://github.com/haipome/fnv/blob/master/fnv.c
    struct String* self = (struct String*) _self;
    return FNV32(self->string_data, self->len);
}


struct StringClass string_class = {
        .class = {
                .magic = MAGIC,
                .size = sizeof(struct String),
                .object_init = __construct__String,
                .get_size = String_get_size,
                .get_len = String_get_len,
                .c_str = String_str,
                .copy = String_copy,
                .append = String_append,
                .hash = String_hash,
                .equals = String_equal,
                .object_name = "String",
                .string = {
                        .string_split = String_split,
                        .string_slice = String_slice

                }
        }

};

void* String = &string_class;
