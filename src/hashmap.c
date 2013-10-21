#include "hashmap.h"

bool rng_seeded = false;
const void *    __construct__HashMap(const void * self, va_list args);
void            print_HashMap(void * _self);
size_t          get_size_HashMap(const void * _self);
size_t          get_len_HashMap(const void * _self);
const char *    str_HashMap(const void *_self);
void *          insert_HashMap(const void * _self, 
                               const void * key, 
                               const void * _other);
void *          get_HashMap(const void * _self, 
                            const void * key);

const void *    copy_HashMap(const void * _self);
const void *    next_HashMap(const void * _self);

void internal_resize_HashMap(struct HashMap * self, uint32_t new_m);

struct HashItem * alloc_hash_items(size_t n){
   struct HashItem * items =  (struct HashItem *) malloc(sizeof(struct HashItem) * n);
   memset (items, 0, n* sizeof(struct HashItem));
   return items;
}


uint32_t random_a(){
    uint32_t a = random() % MAX_A;
    while (! (a &  0x01) ) 
        a =  random() % MAX_A;
    return a;
}

uint32_t lg(uint32_t n){
    uint32_t ans = 0;
    while (n)  {
        n >>=1; 
        ans++;
    }
    return ans - 1;
}

uint32_t random_b(uint32_t m){

    uint32_t M = lg(m);
    return random() % (2 << ( W- M));
}

uint32_t internal_hash(uint32_t a, uint32_t b, uint32_t M, uint32_t key){
    return (uint32_t) (a*key+b) >> (W-M);
}

void  internal_insert_HashMap(struct HashMap * self,
                               uint32_t internal_key,
                               const void * _value) {

    uint32_t h = internal_hash(self->a, self->b, self->M, internal_key);
    struct HashItem * dest =  &self->items[h];
    int depth = 0;
    while(dest->value){
        depth++;
        if (dest->key == internal_key){
            dest->value = _value;
            return;
        }
        if (dest->next)
            dest = dest->next; 
        else {
            dest->next = malloc(sizeof(struct HashItem));
            memset(dest->next, 0, sizeof(struct HashItem));
            dest = dest->next;
        }
    }
    // Increase the length
    self->len++;
    dest->value = _value;
    dest->key = internal_key;

    if (self->len > self->hwm){
        internal_resize_HashMap(self, self->m * 2);
    }

}
void internal_resize_HashMap(struct HashMap * self, uint32_t new_m){
    //hold onto items, so we can insert them into the new structure
    assert( ! (new_m & 0x01)); // make sure it's a power of two

    if (new_m < HASH_TABLE_DEFAULT_LEN) {
        printf("Limiting lower bound to %d\n", HASH_TABLE_DEFAULT_LEN);
        new_m = HASH_TABLE_DEFAULT_LEN; 
    }
    struct HashItem * old_items = self->items;
    uint32_t old_m = self->m;

    // double relavant parameters
    // This is necessary so insertion works
    // If m didn't change, the hash algorithm would put items in old positions
    self->m  = new_m;
    self->M = lg(self->m);
    self->hwm = self->m * HWM_FRACTION;
    self->lwm = self->m * HWM_FRACTION * 0.5;


    // it's also necessary to reset the count since we're essentially making a new 
    // HashMap
    self->len = 0;

    // allocate new space
    self->items = alloc_hash_items(self->m);

    int i; 
    struct HashItem * item;
    bool first_level = true;

    // Use m/2
    for(i=0; i <  old_m; i++) {
        item = &old_items[i];
        first_level = true;

        while( item->value) {
            internal_insert_HashMap(self, item->key, item->value);
            if(item->next){
                struct HashItem * old_item = item; 
                item = item->next;
                if (!first_level)
                    free(old_item);
                first_level = false;
            } else {
                item->value = NULL; 
            }
        }
    }
    free(old_items);

}



size_t get_len_HashMap(const void * _self){
    struct HashMap * self = (struct HashMap *) _self;
    return self->len;
}


size_t get_size_HashMap(const void * _self){
    struct HashMap * self = (struct HashMap *) _self;
    return self->size;
}

const void * __construct__HashMap(const void * _self, va_list args) {
    struct HashMap * self = (struct HashMap *) _self;
    //const char * data = va_arg(args, const char *);
    // for now only support empty constructor
    self->size = sizeof(struct HashMap );
    self->len = 0;
    self->m = HASH_TABLE_DEFAULT_LEN;
    self->M = lg(HASH_TABLE_DEFAULT_LEN);
    //set the highwater mark
    self->hwm = self->m  * HWM_FRACTION;
    self->lwm = 1;
    self->class = HashMap;

    if (!rng_seeded) {
        srand(time(NULL));
    }
    // Initialize random parameters
    self->a = random_a();
    self->b = random_b(HASH_TABLE_DEFAULT_LEN);


    // Initialize the storage for the items
    self->items = alloc_hash_items(self->m);

    self->len = 0;
    return self;
}

void print_HashMap(void * _self) {
    struct HashMap * self = (struct HashMap *) _self;
    printf("{...}");
}


const char * str_HashMap(const void * _self){
    struct HashMap * self = (struct HashMap *) _self;
    fprintf(stderr, "str not implemented");
    exit(EXIT_FAILURE);
}


void * insert_HashMap(const void * _self, 
                      const void * _key, 
                      const void * _value) {
    struct HashMap * self = (struct HashMap *) _self;
    if (get_obj(_key, "Attempted to insert invalid key\n")) {
        if (get_obj(_value, NULL) ){
            const struct class_header * header = get_class_header(_key);
            if (!header->hash) {
                fprintf(stderr, "Attempt to use unhashable type as key\n");
                exit(1);
            }
            uint32_t key = header->hash(_key);
            internal_insert_HashMap(self, key, _value);
            //printf("self['%s'] = '%s'\n", str(_key), str(_value));
        }
    }
}

void * get_HashMap(const void * _self, 
                   const void * _key ) {
    struct HashMap * self = (struct HashMap *) _self;
    if (get_obj(_key, "Attempted to get with  invalid key\n")) {
           const struct class_header * header = get_class_header(_key);
           if (!header->hash) {
               fprintf(stderr, "Attempt to use unhashable type as key\n");
               exit(1);
           }
           uint32_t key = header->hash(_key);
           uint32_t h = internal_hash(self->a, self->b, self->M, key);
           struct HashItem * _item =  &self->items[h];
           int depth;
           while(_item && _item->key != key){
               if (_item->next){
                    _item = _item->next; 
                    depth++;
               }
               else
                   return NULL;
           }
           return _item->value;
        }
    return NULL;
}


void  del_item_HashMap(const void * _self, 
                   const void * _key ) {
    struct HashMap * self = (struct HashMap *) _self;
    if (get_obj(_key, "Attempted to delete an invalid key\n")) {
           const struct class_header * key = get_obj(_key, NULL);
           if (!key->hash) {
               fprintf(stderr, "Attempt to use unhashable type as key\n");
               exit(1);
           }
           uint32_t item_key = key->hash(key);
           uint32_t h = internal_hash(self->a, self->b, self->M, item_key);

           struct HashItem * _item =  &self->items[h];

           struct HashItem * _parent;
           int depth = 0;
           while(_item && _item->key != item_key){
               if (_item->next){
                    _item = _item->next; 
                    _parent = _item;
                    depth++;
               }
               else
                   return;
           }
           if(_item->key == item_key){
               printf("Key match\n");
           } else {
               printf("Keys do not match\n");
           }
           // unlink item from list
           if (_item->next){
               _parent->next = _item->next;
               if (depth)
                   free(_item);
           
            // or zero out so the table knows it's an empty slot
            } else{
                _item->value = NULL;
                _item->key  = 0;
           }
           self->len--;
           if (self->len < self->lwm) {
               internal_resize_HashMap(self, self->m / 2)  ;
           }
        }
}

/* Iterable HashMap methods
 */

const void* iter_HashMap(const void * _self){
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
const void * next_HashMap(const void * _self){
    struct HashMap_iter * self = (struct HashMap_iter *) _self;
    struct HashMap * hash_map  = self->hash_map;
    assert(hash_map);
    if (self->position < self->hash_map->len) {
        while (true) {
            struct HashItem * item_to_yield;
            if (self->current_item->value){
                item_to_yield = self->current_item; 
                if (self->current_item->next){
                    self->current_item = self->current_item->next;
                } else {
                    // if we are at the end of the linked list, jump to the 
                    // next index into the hash table
                    self->index++;
                    self->current_item = &self->hash_map->items[self->index] ;
                }

                self->position++;
                return item_to_yield->value;
            }else {
                // If there is not value in this item, jump to the 
                // next index into the hash table
                self->index++;
                self->current_item = &self->hash_map->items[self->index] ;
                // We haven't found anything so "recurse"
                continue;
            }
        }
       
    } else {
        return 0; 
    }
     
}

const void * __construct__HashMap_iter(const void * _self, va_list args) {

    struct HashMap_iter * self = (struct HashMap_iter *) _self;
    self->class = HashMap_iter;

    self->hash_map =  (struct HashMap *)  va_arg(args, const void *);
    assert(self->hash_map);
    // Initialize the starting conditions for the iterator
    self->position = 0;
    self->index = 0;
    self->current_item = &self->hash_map->items[0];
    return self;
}

const void* copy_HashMap(const void * _self){
    struct HashMap * self = (struct HashMap *) _self;
    return NULL;
}


struct HashMapClass hash_map_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct HashMap),
              .__construct__ = __construct__HashMap,
              .get_size = get_size_HashMap,
              .get_len = get_len_HashMap,
              .str = str_HashMap,
              .copy = copy_HashMap,
              .insert = insert_HashMap,
              .get = get_HashMap,
              .del_item = del_item_HashMap,
              .iter = iter_HashMap
             }

};

// set the HashMap variable to serve as the type of a HashMap
void * HashMap = &hash_map_class;

struct HashMapClass_iter hash_map_class_iter = {
    .class = {.magic = MAGIC,
              .__construct__ = __construct__HashMap_iter,
              .size = sizeof(struct HashMap_iter),
              .next = next_HashMap
             }

};
void * HashMap_iter = &hash_map_class_iter;
