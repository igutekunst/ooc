#include <obj_hashmap.h>

bool rng_seeded = false;
const void * __construct__HashMap(const void * self, va_list args);
void print_HashMap(void * _self);
size_t get_size_HashMap(const void * _self);
size_t get_len_HashMap(const void * _self);
const char * str_HashMap(const void *_self);
void * insert_HashMap(const void * _self, 
                      const void * key, 
                      const void * _other);
void * get_HashMap(const void * _self, 
                      const void * key);
const void * copy_HashMap(const void * _self);

void internal_double_HashMap(struct HashMap * self);

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

void * internal_insert_HashMap(struct HashMap * self,
                               uint32_t internal_key,
                               const void * _value) {

    uint32_t h = internal_hash(self->a, self->b, self->M, internal_key);
    struct HashItem * dest =  &self->items[h];
    int depth = 0;
    while(dest->value){
        depth++;
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
        internal_double_HashMap(self);
    }

}
void internal_double_HashMap(struct HashMap * self){
    //hold onto items, so we can insert them into the new structure
    struct HashItem * old_items = self->items;

    // double relavant parameters
    // This is necessary so insertion works
    // If m didn't change, the hash algorithm would put items in old positions
    self->m  = self->m * 2;
    self->M = lg(self->m);
    self->hwm  *= 2;

    // it's also necessary to reset the count since we're essentially making a new 
    // HashMap
    self->len = 0;


    // allocate new space
    self->items = alloc_hash_items(self->m);

    int i; 
    struct HashItem * item;
    bool first_level = true;

    // Use m/2
    for(i=0; i <  self->m / 2; i++) {
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

struct HashMapClass hash_map_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct HashMap),
              .__construct__ = __construct__HashMap,
              .get_size = get_size_HashMap,
              .get_len = get_len_HashMap,
              .str = str_HashMap,
              .copy = copy_HashMap,
              .insert = insert_HashMap,
              .get = get_HashMap
             }

};


void * HashMap = &hash_map_class;


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
            printf("self['%s'] = '%s'\n", str(_key), str(_value));
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
}

const void* copy_HashMap(const void * _self){
    struct HashMap * self = (struct HashMap *) _self;
    return NULL;
}
