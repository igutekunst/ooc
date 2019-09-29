//
// Created by Isaac Gutekunst on 2019-09-28.
//

#include "ooc/tuple.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ooc/string.h>
#include <ooc/math.h>
#include <assert.h>
#include <ooc/tuple.h>
#include <stdio.h>

#include <ooc/object.h>
#include "object_internal.h"


struct TupleItem {
    const void* value;
};

typedef struct TupleItem TupleItem;

struct Tuple {
    struct ClassHeader* class;
    size_t size;
    size_t len;
    TupleItem* items;
};


struct TupleClass {
    struct ClassHeader class;
};

struct TupleIterator {
    struct ClassHeader* class;
    struct Tuple* tuple;
    bool done;
    size_t index;
};

struct TupleIteratorClass {
    struct ClassHeader class;
};


const void* Tuple_init(const void* _self, size_t argc, va_list args) {
    struct Tuple* self = (struct Tuple*) _self;
    self->len = 0;
    self->size = sizeof(struct Tuple);

    if (argc > 1) {
        self->items = (TupleItem*) malloc(sizeof(TupleItem) * argc);
        for (size_t i = 0; i < argc; i++) {
            self->items[i].value = va_arg(args, const void *);
        }
        self->len = argc;
        self->size = self->size + sizeof(TupleItem) * argc;
    }
    return _self;
}


size_t Tuple_get_size(const void* _self) {
    struct Tuple* self = (struct Tuple*) _self;
    return self->size;
}

size_t Tuple_get_len(const void* _self) {
    struct Tuple* self = (struct Tuple*) _self;
    return self->len;
}

const char* Tuple_to_str(const void* _self) {
    struct Tuple* self = (struct Tuple*) _self;
    // TODO don't leak memory.


    const struct String* out = typed_new(String, "(");
    for (size_t i = 0; i < self->len; i++) {
        // TODO Figure out how to properly quote strings
        out = append(out, to_String(self->items[i].value));
        if (i < self->len - 1) {
            out = append(out, new(String, ", "));
        }
    }
    out = append(out, new(String, ")"));
    return c_str(out);
}


const void* Tuple_get_item(const void* _self, const void* _index) {
    struct Tuple* self = (struct Tuple*) _self;
    const struct ClassHeader* index_class;
    index_class = get_class_header_msg(_index, "Tuple get_item called with invalid index\n");
    if (index_class->math.to_int == NULL) {
        fprintf(stderr, "Tuple get_item called with invalid index of type %s"
                        "Index must be support math.to_int\n", class_name(_index));
        exit(EXIT_FAILURE);
    }

    size_t index = obj_to_int(_index);

    if (index >= self->len || index < 0) {
        fprintf(stderr, "Index %zd is out of range\n", index);
    }

    return self->items[index].value;
}

const void* Tuple_iter(const void* _self) {
    return new(TupleIterator, _self);
}


const void* TupleIterator_init(const void* _self, size_t argc, va_list args) {
    (void) argc;
    struct TupleIterator* self = (struct TupleIterator*) _self;
    // TODO Maybe redundant
    self->class = TupleIterator;

    self->tuple = (struct Tuple*) va_arg(args, const void *);
    assert(self->tuple);
    self->index = 0;

    self->done = false;


    return self;
}


const void* TupleIterator_next(const void* _self) {
    struct TupleIterator* self = (struct TupleIterator*) _self;

    if (self->index >= self->tuple->len) {
        return NULL;
    }
    TupleItem* item = &self->tuple->items[self->index];
    self->index++;
    return item;
}

CompareValue Tuple_compare(const void* _lhs, const void* _rhs) {
    struct Tuple* lhs = (struct Tuple*) _lhs;
    struct Tuple* rhs = (struct Tuple*) _rhs;
    struct ClassHeader* lhs_class = get_class_header_msg(lhs->items[0].value,
                                                         "Tuple_compare failed because item[0] is invalid\n");
    if (lhs_class->compare == NULL) {
        fprintf(stderr, "Tuple compare failed because item[0] does not support compare\n");
        exit(EXIT_FAILURE);
    }

    return compare(lhs->items[0].value, rhs->items[0].value);

}


struct TupleClass Tuple_class = {
        .class = {.magic = MAGIC,
                .size = sizeof(struct Tuple),
                .object_init = Tuple_init,
                .get_size = Tuple_get_size,
                .get_len = Tuple_get_len,
                .c_str = Tuple_to_str,
                .iter = Tuple_iter,
                .get_item = Tuple_get_item,
                .compare = Tuple_compare,
                .object_name = "Tuple"
        }
};

struct TupleIteratorClass tuple_iterator_class = {
        .class = {.magic = MAGIC,
                .object_init = TupleIterator_init,
                .size = sizeof(struct TupleIterator),
                .next = TupleIterator_next
        }

};

void* TupleIterator = &tuple_iterator_class;

void* Tuple = &Tuple_class;


