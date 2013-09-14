#include <obj_string.h>


void * __construct__String(void * self, va_list args);
void print_String(void * _self);
size_t get_size_String(const void const* _self);
size_t get_len_String(const void const* _self);
const char * str_String(const void const *_self);
const void * append_String(const void const * _self, const void *const _other);
const void * copy_String(const void const* _self);
uint32_t hash_String(const void const * _self);

struct StringClass string_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct String),
              .__construct__ = __construct__String,
              .get_size = get_size_String,
              .get_len = get_len_String,
              .str = str_String,
              .copy = copy_String,
              .append = append_String,
              .hash = hash_String
             }

};

void * String = &string_class;

size_t get_len_String(const void const* _self){
    struct String * self = (struct String *) _self;
    return self->len;
}

size_t get_size_String(const void const* _self){
    struct String * self = (struct String *) _self;
    return self->size;
}

void * __construct__String(void * _self, va_list args) {
    struct String * self = (struct String *) _self;
    const char * data = va_arg(args, const char *);
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


const char * str_String(const void const * _self){
    struct String * self = (struct String *) _self;
    return self->data;
}
// Creates a copy of the first string, and
// appends the second string to it
const void * append_String(const void const* _self, const void const* _other){
    struct String * self = (struct String *) _self;
    const struct String * other = (const struct String *) _other;

    if (type(other) == String){

        char * new_char_data = (char *) malloc(len(self) + len(other) );

        strcpy(new_char_data, self->data);
        strcat(new_char_data, other->data);
        
        const struct String * new_string = new(String, new_char_data);
        return new_string;
        
        //
        self = realloc(self, size(self) + len(other));
        self->size += len(other);
        self->len += len(other);
        void * new_data = (void *) &new_string[1];

        // Since we are pointing new_string->data 
        // at memory allocated at the end of self,
        // we may need to update the pointer if realloc 
        // moved the whole block, since self->data now points
        // to an address in the previous block over memory

        if (self->data != new_data) {
            strcpy(new_data, self->data);
            self->data = new_data;
        }

        self->data = strcat(self->data, other->data);
        return self;

    }else {
        printf("Cannot append some type to string\n") ;
        exit(1);
    }
}

const void* copy_String(const void const* _self){
    struct String * self = (struct String *) _self;
    return new(String, self->data);
}
