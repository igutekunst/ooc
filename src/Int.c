#include <ooc/Int.h>
#include "object_internal.h"


struct IntClass{
    struct class_header class;
};

const void * __construct__Int(const void * self, va_list args);
void print_Int(const void * _self);
size_t get_size_Int(const void * _self);
const char * to_str_Int(const void * _self);

void * append_Int(void * _self, void * _other);

const void* Int_add(const void* lhs, const void* rhs);
const void* Int_sub(const void* lhs, const void* rhs);
const void* Int_mul(const void* lhs, const void* rhs);
const void* Int_div(const void* lhs, const void* rhs);

struct IntClass int_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct Int),
              .object_init = __construct__Int,
              .print = print_Int,
              .get_size = get_size_Int,
              .str = to_str_Int,
              .math  =  {.enabled = true,
                         .add = Int_add,
                         .sub = Int_sub,
                         .mul = Int_mul,
                         .div = Int_div,
                        }
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
    self->str_value;
    self->value = value;
    snprintf(self->str_value, sizeof(self->str_value), "%d", self->value);
    self->size = sizeof(struct Int);
    return self;
}

void print_Int(const void * _self) {
    struct Int * self = (struct Int *) _self;
    printf("%d", self->value);
}


const char * to_str_Int(const void * _self){
    struct Int * self = (struct Int *) _self;
    if (self->str_value)
        return self->str_value;
    return NULL;

}

const void* Int_add(const void* _lhs, const void* _rhs) {
    struct Int * lhs = (struct Int *)_lhs;
    struct Int * rhs = (struct Int *)_rhs;
    return new(Int, lhs->value + rhs->value);
}

const void* Int_sub(const void* _lhs, const void* _rhs) {
    struct Int * lhs = (struct Int *)_lhs;
    struct Int * rhs = (struct Int *)_lhs;
    return new(Int, lhs->value - rhs->value);

}

const void* Int_mul(const void* _lhs, const void* _rhs) {
    struct Int * lhs = (struct Int *)_lhs;
    struct Int * rhs = (struct Int *)_lhs;
    return new(Int, lhs->value * rhs->value);

}

const void* Int_div(const void* _lhs, const void* _rhs) {
    struct Int * lhs = (struct Int *)_lhs;
    struct Int * rhs = (struct Int *)_lhs;
    return new(Int, lhs->value / rhs->value);

}


void test_int() {

}
