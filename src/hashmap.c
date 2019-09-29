#include "object_internal.h"
#include <ooc/hashmap.h>


struct HashItem {
    uint64_t internal_key;
    const void *key;
    const void *value;
    struct HashItem *next;
    bool present;
};
typedef struct HashItem HashItem;

struct HashMap {
    struct ClassHeader *class;
    size_t size;
    size_t len;
    uint64_t m;
    uint64_t M;
    uint64_t hwm;
    uint64_t lwm;
    uint64_t a;
    uint64_t b;
    bool debug;
    struct HashItem *items;

};

struct HashMap_iter {
    struct ClassHeader *class;
    struct HashItem *current_item;
    int index;
    struct HashMap *hash_map;
    int position;
};

// Hash Parameters.
// See https://en.wikipedia.org/wiki/Universal_hashing
// For explanations of parameters
#define W   64
#define HASH_TABLE_DEFAULT_LEN 4
#define MAX_A  18446744073709551615UL
#define HWM_FRACTION 0.5

struct HashMapClass {
    struct ClassHeader class;
};

struct HashMapClass_iter {
    struct ClassHeader class;
};

bool rng_seeded = false;

const void *__construct__HashMap(const void *_self, size_t argc, va_list args);

void print_HashMap(void *_self);

size_t get_size_HashMap(const void *_self);

size_t get_len_HashMap(const void *_self);

const char *str_HashMap(const void *_self);

void insert_HashMap(const void *_self,
                    const void *key,
                    const void *_value);

const void *get_HashMap(const void *_self,
                        const void *key);

const void *copy_HashMap(const void *_self);

const void *next_HashMap(const void *_self);

void internal_resize_HashMap(struct HashMap *self, uint64_t new_m);

void internal_print_table(struct HashMap *self);

struct HashItem *alloc_hash_items(size_t n) {
    struct HashItem *items = (struct HashItem *) malloc(sizeof(struct HashItem) * n);
    memset (items, 0, n * sizeof(struct HashItem));
    return items;
}


uint64_t random_a() {
    uint64_t a = random() % MAX_A;
    while (!(a & 0x01U))
        a = random() % MAX_A;
    return a;
}

uint64_t lg(uint64_t n) {
    uint64_t ans = 0;
    while (n) {
        n >>= 1U;
        ans++;
    }
    return ans - 1U;
}

uint64_t random_b(uint64_t m) {

    uint64_t M = lg(m);
    // TODO random might return a negative number?
    return (uint64_t) random() % (2U << (W - M));
}

uint64_t internal_hash(uint64_t a, uint64_t b, uint64_t M, uint64_t key) {
    uint64_t h = (uint64_t) (a * key + b) >> (W - M);
    return h;
}

void internal_insert_HashMap(struct HashMap *self,
                             const void *_key,
                             const void *_value) {

    const struct ClassHeader *header = get_class_header(_key);
    uint64_t internal_key = header->hash(_key);
    uint64_t h = internal_hash(self->a, self->b, self->M, internal_key);
    struct HashItem *dest = &self->items[h];
    size_t depth = 0;
    assert(h < self->m);
    while (dest->present) {
        if (dest->internal_key == internal_key) {
            if (equals(dest->key, _key)) {
                dest->value = _value;
                dest->present = true;
                return;
            }
        }
        if (dest->next) {
            dest = dest->next;
        } else {
            dest->next = malloc(sizeof(struct HashItem));
            memset(dest->next, 0, sizeof(struct HashItem));
            dest = dest->next;
        }
        depth++;
    }

    if (self->debug) {
        printf("Inserted key %llu into row %llu, depth %zu\n", internal_key, h, depth);
    }

    // Increase the length
    self->len++;
    dest->value = _value;
    dest->key = _key;
    dest->present = true;
    dest->internal_key = internal_key;

    if (self->len > self->hwm) {
        internal_resize_HashMap(self, self->m * 2);
    }

}

void internal_print_table(struct HashMap *self) {

    size_t count = 0;
    struct HashItem *item;

    printf("Row | First Key    pn |\n");
    printf("----+-----------------|\n");
    for (int i = 0; i < self->m; i++) {
        item = &self->items[i];
        printf("%03d | ", i);
        while (item->present) {
            printf(" %11llu %c%c | -> ", item->internal_key, "-x"[item->present], "-x"[item->next != NULL]);
            count++;
            if (item->next != NULL) {
                item = item->next;
            } else {
                break;
            }
        }
        printf("   Empty        | \n");
    }
}

size_t internal_count_HashMap(struct HashMap *self) {

    size_t count = 0;
    struct HashItem *item;

    for (int i = 0; i < self->m; i++) {
        item = &self->items[i];
        while (item->present) {
            count++;
            if (item->next) {
                item = item->next;
            } else {
                break;
            }
        }
    }
    return count;
}

void internal_resize_HashMap(struct HashMap *self, uint64_t new_m) {
    //hold onto items, so we can insert them into the new structure
    // TODO this is wrong
    assert(!(new_m & 0x01U)); // make sure it's a power of two

    if (new_m < HASH_TABLE_DEFAULT_LEN) {
        if (self->debug) {
            printf("Limiting lower bound to %d\n", HASH_TABLE_DEFAULT_LEN);
        }
        new_m = HASH_TABLE_DEFAULT_LEN;
    }

    if (self->debug) {
        printf("Resizing from %llu to %llu\n", self->m, new_m);
        internal_print_table(self);
    }

    struct HashItem *old_items = self->items;
    uint64_t old_m = self->m;

    // This is necessary so insertion works
    // If m didn't change, the hash algorithm would put items in old positions
    self->m = new_m;
    self->M = lg(self->m);
    self->hwm = (uint64_t) (self->m * HWM_FRACTION);
    self->lwm = (uint64_t) (self->m * HWM_FRACTION * 0.5);

    self->a = random_a();
    self->b = random_b(self->m);


    // it's also necessary to reset the count since we're essentially making a new
    // HashMap
    self->len = 0;

    // allocate new space
    self->items = alloc_hash_items(self->m);

    struct HashItem *item;

    size_t inserted = 0;
    for (int i = 0; i < old_m; i++) {
        item = &old_items[i];
        size_t depth = 0;
        while (item->present) {
            internal_insert_HashMap(self, item->key, item->value);
            inserted++;
            if (item->next) {
                item = item->next;
                depth += 1;
            } else {
                break;
            }
        }
    }

    // TODO actually free properly. Linked list is not traversed
    free(old_items);

}


size_t get_len_HashMap(const void *_self) {
    struct HashMap *self = (struct HashMap *) _self;
    if (self->debug) {
        size_t counted_size = internal_count_HashMap(self);
        if (counted_size != self->len) {
            printf("Size error: len = %zu, manual count = %zu\n", self->len, counted_size);
            internal_print_table(self);
        }
        assert(self->len == counted_size);
    }
    return self->len;
}


size_t get_size_HashMap(const void *_self) {
    struct HashMap *self = (struct HashMap *) _self;
    return self->size + self->len * sizeof(HashItem);
}

const void *__construct__HashMap(const void *_self, size_t argc, va_list args) {
    //TODO support interesting constructor stuff
    (void) argc;
    (void) args;

    struct HashMap *self = (struct HashMap *) _self;
    //const char * data = va_arg(args, const char *);
    // for now only support empty constructor
    self->size = sizeof(struct HashMap);
    self->len = 0;
    self->m = HASH_TABLE_DEFAULT_LEN;
    self->M = lg(HASH_TABLE_DEFAULT_LEN);
    //set the high water mark
    self->hwm = self->m * HWM_FRACTION;
    self->lwm = 1;
    self->class = HashMap;
    self->debug = false;

    if (!rng_seeded) {
        srandom(time(NULL));
    }
    self->a = random_a();
    self->b = random_b(HASH_TABLE_DEFAULT_LEN);


    // Initialize the storage for the items
    self->items = alloc_hash_items(self->m);
    self->len = 0;
    return self;
}


void __destruct__HashMap(const void *_self) {
    struct HashMap *self = (struct HashMap *) _self;
}

void print_HashMap(void *_self) {
    struct HashMap *self = (struct HashMap *) _self;
    printf("{...}");
}


const char *str_HashMap(const void *_self) {
    struct HashMap *self = (struct HashMap *) _self;
    fprintf(stderr, "str not implemented");
    exit(EXIT_FAILURE);
}


void insert_HashMap(const void *_self,
                    const void *_key,
                    const void *_value) {
    struct HashMap *self = (struct HashMap *) _self;
    if (get_class_header_msg(_key, "Attempted to insert invalid key\n")) {
        if (get_class_header_msg(_value, NULL)) {
            const struct ClassHeader *header = get_class_header(_key);
            if (!header->hash) {
                fprintf(stderr, "Attempt to use unhashable type as key\n");
                exit(1);
            }
            if (self->debug) {
                printf("self['%s'] = '%s'\n", c_str(_key), c_str(_value));
            }
            internal_insert_HashMap(self, _key, _value);
        }
    }
}

const void *get_HashMap(const void *_self,
                        const void *_key) {
    struct HashMap *self = (struct HashMap *) _self;
    if (get_class_header_msg(_key, "Attempted to get_item with  invalid key\n")) {
        const struct ClassHeader *header = get_class_header(_key);
        if (!header->hash) {
            fprintf(stderr, "%s does not support hashing\n", class_name(_key));
            exit(1);
        }
        uint64_t internal_key = header->hash(_key);
        uint64_t h = internal_hash(self->a, self->b, self->M, internal_key);
        struct HashItem *item = &self->items[h];
        int depth = 0;
        while (item) {
            if (item->internal_key == internal_key
                && equals(item->key, _key)) {
                return item->value;
            } else if (item->next) {
                item = item->next;
                depth++;
            } else {
                return NULL;
            }
        }

    }
    // TODO make a 'contains' or 'in' function to test without causing an error
    fprintf(stderr, "KeyError: %s", c_str(_key));
    exit(EXIT_FAILURE);
}


void del_item_HashMap(const void *_self,
                      const void *_key) {

    struct HashMap *self = (struct HashMap *) _self;
    if (get_class_header_msg(_key, "Attempted to delete an invalid key\n")) {
        const struct ClassHeader *key = get_class_header_msg(_key, NULL);
        if (!key->hash) {
            fprintf(stderr, "%s does not support hashing\n", class_name(_self));
            exit(1);
        }

        size_t len_at_start = self->len;
        uint64_t item_key = key->hash(_key);
        uint64_t h = internal_hash(self->a, self->b, self->M, item_key);

        if (self->debug) {
            printf("Attempting to delete %llu\n", item_key);
            internal_print_table(self);
        }

        struct HashItem *item = &self->items[h];

        struct HashItem *parent = NULL;
        int depth = 0;
        while (item && item->internal_key != item_key) {
            if (item->next) {
                parent = item;
                item = item->next;
                depth++;
            } else {
                return;
            }
        }

        // TODO maybe no need to assert here
        assert(item);

        if (parent) {
            if (self->debug) {
                printf("Parent: %llu %c %c\n", parent->internal_key, "-x"[parent->present], "-x"[parent->next != NULL]);
                printf("Item: %llu %c %c\n", item->internal_key, "-x"[item->present], "-x"[item->next != NULL]);
            }

            parent->next = item->next;
            if (depth > 0) {
                free(item);
            }
        } else {
            if (item->next) {
                struct HashItem *item_to_free = item->next;
                *item = *(item->next);
                free(item_to_free);
            } else {
                item->present = false;
                item->next = NULL;
                item->value = NULL;
            }
        }

        if (self->debug) {
            printf("Deleted %llu from row %llu depth %d\n", item->internal_key, h, depth);
            internal_print_table(self);
        }

        self->len--;
        if (self->len < self->lwm) {
            internal_resize_HashMap(self, self->m / 2);
        }
        assert(self->len == len_at_start - 1);
    }
}

/* Iterable HashMap methods
 */

const void *iter_HashMap(const void *_self) {
    return new(HashMap_iter, _self);
}


// Implementation notes
// The iterator needs to remember the place in the iteration
// For performance reasons, it should cache 
//      A pointer the current  item in the current linked list
//      An index into the hash table
//
// Initially, the  pointer will be initialized to the first item in the table (could be empty)
//          and the index will be zero
/**
 * @internal
 * @private
 * @param _self
 * @return
 */
const void *next_HashMap(const void *_self) {
    struct HashMap_iter *self = (struct HashMap_iter *) _self;
    struct HashMap *hash_map = self->hash_map;
    assert(hash_map);
    if (self->position < self->hash_map->len) {
        while (true) {
            struct HashItem *item_to_yield;
            if (self->current_item->present) {
                item_to_yield = self->current_item;
                if (self->current_item->next) {
                    self->current_item = self->current_item->next;
                } else {
                    // if we are at the end of the linked list, jump to the 
                    // next index into the hash table
                    self->index++;
                    self->current_item = &self->hash_map->items[self->index];
                }

                self->position++;
                return item_to_yield->key;
            } else {
                // If there is not value in this item, jump to the 
                // next index into the hash table
                self->index++;
                self->current_item = &self->hash_map->items[self->index];
                // We haven't found anything so "recurse"
                continue;
            }
        }

    } else {
        return 0;
    }

}

const void *__construct__HashMap_iter(const void *_self, size_t argc, va_list args) {

    struct HashMap_iter *self = (struct HashMap_iter *) _self;
    self->class = HashMap_iter;

    self->hash_map = (struct HashMap *) va_arg(args, const void *);
    assert(self->hash_map);
    // Initialize the starting conditions for the iterator
    self->position = 0;
    self->index = 0;
    self->current_item = &self->hash_map->items[0];
    return self;
}

const void *copy_HashMap(const void *_self) {
    struct HashMap *self = (struct HashMap *) _self;
    return NULL;
}

/** @private */
struct HashMapClass hash_map_class = {
        .class = {.magic = MAGIC,
                .size = sizeof(struct HashMap),
                .object_init = __construct__HashMap,
                .object_deinit = __destruct__HashMap,
                .get_size = get_size_HashMap,
                .get_len = get_len_HashMap,
                .c_str = str_HashMap,
                .copy = copy_HashMap,
                .insert = insert_HashMap,
                .get_item = get_HashMap,
                .del_item = del_item_HashMap,
                .iter = iter_HashMap,
                .object_name = "HashMap"
        }

};

// set the HashMap variable to serve as the type of a HashMap
void *HashMap = &hash_map_class;

static struct HashMapClass_iter hash_map_class_iter = {
        .class = {.magic = MAGIC,
                .object_init = __construct__HashMap_iter,
                .size = sizeof(struct HashMap_iter),
                .next = next_HashMap
        }

};
void *HashMap_iter = &hash_map_class_iter;
