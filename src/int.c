#include <ooc/int.h>
#include "object_internal.h"
#include <ooc/object.h>
#include <string.h>
#include <stdint.h>
#define MAX_STR_SIZE 20
struct Int{
    struct class_header * class;
    size_t size;
    int value;
    char str_value[MAX_STR_SIZE];
};

struct IntClass {
    struct class_header class;
};

const void *Int_init(const void *_self, size_t argc, va_list args);
void Int_print(const void* _self);
size_t Int_get_size(const void* _self);
const char* Int_to_str(const void* _self);

const void* Int_add(const void* lhs, const void* rhs);
const void* Int_sub(const void* lhs, const void* rhs);
const void* Int_mul(const void* lhs, const void* rhs);
const void* Int_div(const void* lhs, const void* rhs);
int Int_to_int(const void* _self);


size_t Int_get_size(const void* _self) {
    struct Int* self = (struct Int*) _self;
    return self->size;
}

const void *Int_init(const void *_self, size_t argc, va_list args) {
    struct Int* self = (struct Int*) _self;
    uint32_t value = va_arg(args, uint32_t);
    self->class = Int;
    self->str_value[0] = '\0';
    self->value = value;
    self->size = sizeof(struct Int);
    return self;
}

void Int_print(const void* _self) {
    struct Int* self = (struct Int*) _self;
    printf("%d", self->value);
}


const char* Int_to_str(const void* _self) {
    struct Int* self = (struct Int*) _self;
    if (self->str_value[0] == '\0') {
        snprintf(self->str_value, sizeof(self->str_value), "%d", self->value);
    }
        return self->str_value;
    return NULL;

}

const void* Int_add(const void* _lhs, const void* _rhs) {
    struct Int* lhs = (struct Int*) _lhs;
    struct Int* rhs = (struct Int*) _rhs;
    return new(Int, lhs->value + rhs->value);
}

const void* Int_sub(const void* _lhs, const void* _rhs) {
    struct Int* lhs = (struct Int*) _lhs;
    struct Int* rhs = (struct Int*) _lhs;
    return new(Int, lhs->value - rhs->value);

}

const void* Int_mul(const void* _lhs, const void* _rhs) {
    struct Int* lhs = (struct Int*) _lhs;
    struct Int* rhs = (struct Int*) _lhs;
    return new(Int, lhs->value * rhs->value);

}

const void* Int_div(const void* _lhs, const void* _rhs) {
    struct Int* lhs = (struct Int*) _lhs;
    struct Int* rhs = (struct Int*) _lhs;
    return new(Int, lhs->value / rhs->value);

}

int Int_to_int(const void* _self) {
    struct Int* self = (struct Int*) _self;
    return self->value;
}

CompareValue Int_compare(const void* _lhs, const void* _rhs) {
    struct Int* lhs = (struct Int*) _lhs;
    struct Int* rhs = (struct Int*) _rhs;
    if (lhs->value > rhs->value) {
       return COMPARE_GT;
    } else if (lhs->value == rhs->value){
       return COMPARE_EQ;
    } else {
        return COMPARE_LT;
    }
}


struct IntClass int_class = {
        .class = {.magic = MAGIC,
                .size = sizeof(struct Int),
                .object_init = Int_init,
                .print = Int_print,
                .get_size = Int_get_size,
                .c_str = Int_to_str,
                .object_name = "Int",
                .compare = Int_compare,
                .math  =  {.enabled = true,
                        .add = Int_add,
                        .sub = Int_sub,
                        .mul = Int_mul,
                        .div = Int_div,
                        .to_int = Int_to_int
                }
        }

};

void* Int = &int_class;
