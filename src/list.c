//
// Created by Isaac Gutekunst on 9/28/19.
//

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ooc/string.h>
#include <ooc/math.h>
#include <assert.h>

#include "ooc/list.h"
#include "object_internal.h"


struct ListItem {
    const void* value;
    struct ListItem* next;
};
typedef struct ListItem ListItem;

struct List {
    struct class_header* class;
    size_t size;
    size_t len;
    struct ListItem* head;
    struct ListItem* tail;
};


struct ListClass {
    struct class_header class;
};

const void* List_init(const void* _self, va_list args) {
    struct List* self = (struct List*) _self;
    self->len = 0;
    self->size = sizeof(struct List);
    self->head = NULL;
    self->tail = NULL;
    return _self;
}

void List_print(const void* _self) {

}

size_t List_get_size(const void* _self) {
    struct List* self = (struct List*) _self;
    //TODO update once list grows
    return self->class->size;
}

size_t List_get_len(const void* _self) {
    struct List* self = (struct List*) _self;
    return self->len;
}

const char* List_to_str(const void* _self) {
    struct List* self = (struct List*) _self;
    // TODO don't leak memory.

    /* // This next part leaks memory like crazy. It's only for a POC
     * // Future versions will support reference counting, so this would look like
     * void* frame = alloc(1);
     * out = new_rc(pool, String, "[");
     * ...
     *
     * // In this case, each _rc function will retain a pointer to any allocated
     * // objects in the first argument (frame)
     *
     * out = append_rc(frame, new_rc(frame, String, "\""));
     * // We don't want to free the memory since we want to
     * // return a string, so we retain (increase reference count)
     * // before releasing the frame
     * retain(out);
     *
     * // This will decrease the refcount, and free all the intermediate strings
     *    used above.
     *    Alternatively, there could be an append_destructive, that modifies the first
     *    argument. This is how lists work in python, but not strings.
     * release(frame);
     * return out;
     */

    ListItem* item = self->head;
    const struct String* out = typed_new(String, "[");
    while (item != NULL) {
        out = append(out, new(String, "\""));
        out = append(out, to_String(item->value));
        out = append(out, new(String, "\""));
        if (item->next != NULL) {
            out = append(out, new(String, ", "));
        }
        item = item->next;
    }
    out = append(out, new(String, "]"));
    return str(out);
    return "[]";
}

const void* List_append(const void* _self, const void* _other) {
    struct List* self = (struct List*) _self;
    ListItem* new_item = (ListItem*) malloc(sizeof(ListItem));
    new_item->value = _other;
    new_item->next = NULL;

    if (self->head == NULL) {
        assert(self->tail == NULL);
        self->head = self->tail = new_item;
    } else {

        self->tail->next = new_item;
        self->tail = new_item;
    }

    self->len++;
    self->size += sizeof(ListItem);
    return _self;
}


const void* List_get_item(const void* _self, const void* _index) {
    struct List* self = (struct List*) _self;
    const struct class_header * index_class;
    index_class = get_class_header_msg(_index, "List get_item called with invalid index\n");
    if (index_class->math.to_int == NULL) {
        fprintf(stderr, "List get_item called with invalid index of type %s"
                        "Index must be support math.to_int\n", clsname(_index));
        exit(EXIT_FAILURE);
    }

    size_t index = obj_to_int(_index);
    ListItem* item = self->head;
    const struct String* out = typed_new(String, "[");

    if (index >= len) {
        fprintf(stderr, "Index %d is out of range\n", index);
    }

    for(size_t i = 0; i < index; i++) {
        assert(item != NULL);
        item = item->next;
    }
    return item->value;

}

struct ListClass List_class = {
        .class = {.magic = MAGIC,
                .size = sizeof(struct List),
                .object_init = List_init,
                .print = List_print,
                .get_size = List_get_size,
                .get_len = List_get_len,
                .str = List_to_str,
                .append = List_append,
                .iter = List_iter,
                .get_item = List_get_item,
                .object_name = "List"
        }
};

void* List = &List_class;

