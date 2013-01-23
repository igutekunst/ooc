#include "string.h"


void * __construct__String(void * self, va_list args);
void print_String(void * _self);
size_t get_size_String(void * _self);
size_t get_len_String(void * _self);
char * to_string_String(void * _self);

void * append_String(void * _self, void * _other);

struct StringClass string_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct String),
              .__construct__ = __construct__String,
              .print = print_String,
              .get_size = get_size_String,
              .get_len = get_len_String,
              .to_string = to_string_String,
              .append = append_String
             }

};

void * String = &string_class;

size_t get_len_String(void * _self){
    struct String * self = (struct String *) _self;
    return self->len;
}

size_t get_size_String(void * _self){
    struct String * self = (struct String *) _self;
    return self->size;
}

void * __construct__String(void * _self, va_list args) {
    struct String * self = (struct String *) _self;
    char * data = va_arg(args, char *);
    size_t len = strlen(data);
    size_t size = sizeof(struct String)  + len + 1;
    self = (struct String *) realloc(self, size);
    self->class = String;
    void * space =  (void *) &self[1];
    self->data = strcpy(space, data);
    self->size = size;
    self->len = len;
    return self;
}
void print_String(void * _self) {
    struct String * self = (struct String *) _self;
    printf("%s", self->data);
}


char * to_string_String(void * _self){
    struct String * self = (struct String *) _self;
    return self->data;
}

void * append_String(void * _self, void * _other){
    struct String * self = (struct String *) _self;
    struct String * other = (struct String *) _other;
    if (type(other) == String){
        self = realloc(self, size(self) + len(other));
        self->size += len(other);
        self->len += len(other);
        strcat(self->data, other->data);
        return self;
    }else {
        printf("Cannot append some type to string\n") ;
        exit(1);
    }
}
