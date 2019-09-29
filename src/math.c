#include <ooc/math.h>
#include <ooc/object.h>
#include "object_internal.h"

const void* obj_add(const void* _lhs, const void* _rhs) {
    struct ClassHeader* lhs_header = get_class_header_msg(_lhs, "add argument lhs is invalid\n");
    struct ClassHeader* rhs_header = get_class_header_msg(_rhs, "add argument rhs is invalid\n");

    // TODO consider better type checking
    if (lhs_header->math.add && rhs_header->math.add) {
        return lhs_header->math.add(_lhs, _rhs);
    }

    fprintf(stderr, "TypeError: Could not add %s and %s", class_name(_lhs), class_name(_rhs));
    exit(EXIT_FAILURE);
}

const void* obj_sub(const void* lhs, const void* rhs) {

    fprintf(stderr, "NotImplemented: obj_sub\n");
    exit(EXIT_FAILURE);
}

const void* obj_mul(const void* lhs, const void* rhs) {
    fprintf(stderr, "NotImplemented: obj_mul\n");
    exit(EXIT_FAILURE);
}

const void* obj_div(const void* lhs, const void* rhs) {
    fprintf(stderr, "NotImplemented: obj_div\n");
    exit(EXIT_FAILURE);
}

int obj_to_int(const void* _self) {
    struct ClassHeader* self = get_class_header_msg(_self, "add argument lhs is invalid\n");
    if (self->math.to_int) {
        return self->math.to_int(_self);
    }

    fprintf(stderr, "%s does not support obj_to_int\n", class_name(_self));
    exit(EXIT_FAILURE);
}

