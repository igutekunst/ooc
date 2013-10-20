
#include "Int.h"

const void * __construct__Int(const void * self, va_list args);
void print_Int(const void * _self);
size_t get_size_Int(const void * _self);
const char * to_str_Int(const void * _self);

void * append_Int(void * _self, void * _other);

struct IntClass int_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct Int),
              .__construct__ = __construct__Int,
              .print = print_Int,
              .get_size = get_size_Int,
              .str = to_str_Int
             }

};

void * Int = &int_class;

size_t get_size_Int(const void * _self){
    struct Int * self = (struct Int *) _self;
    return self->size;
}

const void * __construct__Int(const void * _self, va_list args) {
    struct Int * self = (struct Int *) _self;
    uint32_t  value = va_arg(args, uint32_t);
    self->class = Int;
    self->str_value = NULL;
    self->value = value;
    self->size = sizeof(struct Int);
    return self;
}

void print_Int(const void * _self) {
    struct Int * self = (struct Int *) _self;
    printf("%d", self->value);
}


const char * to_str_Int(const void * _self){
    struct Int * self = (struct Int *) _self;
    printf("printing int\n");
    if (self->str_value)
        return self->str_value;
    return NULL;

}



void test_int() {

}
