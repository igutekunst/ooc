#include <ooc/trait_math.h>
#include <ooc/object.h>
#include "object_internal.h"

const void* obj_add(const void* _lhs, const void* _rhs) {
    struct class_header * lhs_header = get_obj(_lhs, "add argument lhs is invalid\n");
    struct class_header * rhs_header = get_obj(_rhs, "add argument rhs is invalid\n");

    // TODO consider better type checking
    if (lhs_header->math.add
        && rhs_header->math.add
    ) {
        return lhs_header->math.add(_lhs, _rhs);
    } else {
       fprintf(stderr, "TypeError: Could not add %s and %s", clsname(_lhs), clsname(_rhs) );
    }
}

const void* obj_sub(const void* lhs, const void* rhs) {

}

const void* obj_mul(const void* lhs, const void* rhs) {

}

const void* obj_div(const void* lhs, const void* rhs) {

}
