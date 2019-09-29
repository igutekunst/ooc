//
// Created by Isaac Gutekunst on 9/28/19.
//

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ooc/string.h>
#include <ooc/math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ooc/object.h>

#include "ooc/list.h"
#include "object_internal.h"


struct ListItem {
    const void* value;
    struct ListItem* next;
};

typedef struct ListItem ListItem;

struct List {
    struct ClassHeader* class;
    size_t size;
    size_t len;
    bool iterating;
    struct ListItem* head;
    struct ListItem* tail;
};


struct ListClass {
    struct ClassHeader class;
};

struct ListIterator {
    struct ClassHeader* class;
    struct List* list;
    bool done;
    ListItem* item;
};

struct ListIteratorClass {
    struct ClassHeader class;
};


const void* List_append(const void* _self, const void* _other);


void MergeSort(struct ListItem** headRef, SortDirection sort_direction);


const void* List_init(const void* _self, size_t argc, va_list args) {
    struct List* self = (struct List*) _self;
    self->len = 0;
    self->size = sizeof(struct List);
    self->head = NULL;
    self->tail = NULL;
    self->iterating = false;

    if (argc > 1) {
        for (size_t i = 0; i < argc; i++) {
            const void* item = va_arg(args, const void*);
            List_append(_self, item);
        }
    }
    return _self;
}


size_t List_get_size(const void* _self) {
    struct List* self = (struct List*) _self;
    return self->size;
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
        //TODO figure out string quotes
        out = append(out, to_String(item->value));
        if (item->next != NULL) {
            out = append(out, new(String, ", "));
        }
        item = item->next;
    }
    out = append(out, new(String, "]"));
    return c_str(out);
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
    const struct ClassHeader* index_class;
    index_class = get_class_header_msg(_index, "List get_item called with invalid index\n");
    if (index_class->math.to_int == NULL) {
        fprintf(stderr, "List get_item called with invalid index of type %s"
                        "Index must be support math.to_int\n", class_name(_index));
        exit(EXIT_FAILURE);
    }

    ssize_t index = obj_to_int(_index);

    ListItem* item = self->head;
    const struct String* out = typed_new(String, "[");

    if (index >= self->len || index < 0) {
        fprintf(stderr, "Index %zd is out of range\n", index);
    }

    for (size_t i = 0; i < index; i++) {
        assert(item != NULL);
        item = item->next;
    }
    return item->value;

}

const void* List_iter(const void* _self) {
    return new(ListIterator, _self);
}


const void* ListIterator_init(const void* _self, size_t argc, va_list args) {
    (void) argc;

    struct ListIterator* self = (struct ListIterator*) _self;
    // TODO Maybe redundant
    self->class = ListIterator;

    self->list = (struct List*) va_arg(args, const void *);
    assert(self->list);

    self->list->iterating = true;

    self->item = self->list->head;
    self->done = false;


    return self;
}


const void* ListIterator_next(const void* _self) {
    struct ListIterator* self = (struct ListIterator*) _self;

    ListItem* item = NULL;
    if (self->done != true) {

        if (self->item == NULL) {
            self->list->iterating = false;
            self->done = true;
        } else {
            item = self->item;
            self->item = self->item->next;
        }
    }
    if (item) {
        return item->value;
    }
    return NULL;
}

void List_sort(const void* _self, SortDirection sort_direction) {
    (void) sort_direction;
    struct List* self = (struct List*) _self;
    MergeSort(&self->head, sort_direction);
}


// List Sorting
// Adapted from https://www.geeksforgeeks.org/merge-sort-for-linked-list/

struct ListItem* SortedMerge(struct ListItem* a, struct ListItem* b, SortDirection sort_direction);


void FrontBackSplit(struct ListItem* source,
                    struct ListItem** frontRef, struct ListItem** backRef);


/* sorts the linked list by changing next pointers (not data) */
void MergeSort(struct ListItem** headRef, SortDirection sort_direction) {
    struct ListItem* head = *headRef;
    struct ListItem* a;
    struct ListItem* b;

    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    /* Split head into 'a' and 'b' sub-lists */
    FrontBackSplit(head, &a, &b);

    /* Recursively obj_sort the sub-lists */
    MergeSort(&a, sort_direction);
    MergeSort(&b, sort_direction);

    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b, sort_direction);
}

/* See https:// www.geeksforgeeks.org/?p=3622 for details of this
function */
struct ListItem* SortedMerge(struct ListItem* a, struct ListItem* b, SortDirection sort_direction) {
    struct ListItem* result = NULL;

    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    /* Pick either a or b, and recur */
    CompareValue c = compare(a->value, b->value);

    // TODO see if this logic can be simplified
    bool a_first;
    if (sort_direction == SORT_ASCENDING) {
        a_first = (c == COMPARE_LT || c == COMPARE_EQ);
    } else {
        a_first = (c == COMPARE_GT || c == COMPARE_EQ);
    }

    if (a_first) {
        result = a;
        result->next = SortedMerge(a->next, b, sort_direction);
    } else {
        result = b;
        result->next = SortedMerge(a, b->next, sort_direction);
    }
    return (result);
}

/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
	and return the two lists using the reference parameters.
	If the length is odd, the extra node should go in the front list.
	Uses the fast/slow pointer strategy. */
void FrontBackSplit(struct ListItem* source,
                    struct ListItem** frontRef, struct ListItem** backRef) {
    struct ListItem* fast;
    struct ListItem* slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}


struct ListClass List_class = {
        .class = {.magic = MAGIC,
                .size = sizeof(struct List),
                .object_init = List_init,
                .get_size = List_get_size,
                .get_len = List_get_len,
                .c_str = List_to_str,
                .sort = List_sort,
                .append = List_append,
                .iter = List_iter,
                .get_item = List_get_item,
                .object_name = "List"
        }
};

struct ListIteratorClass list_iterator_class = {
        .class = {.magic = MAGIC,
                .object_init = ListIterator_init,
                .size = sizeof(struct ListIterator),
                .next = ListIterator_next
        }

};

void* ListIterator = &list_iterator_class;

void* List = &List_class;


