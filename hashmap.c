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
const void * copy_HashMap(const void * _self);


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
uint32_t hash_internal(uint32_t a, uint32_t b, uint32_t M, uint32_t key){
    return  (unsigned) (a*key+b) >> (W-M);
}

struct HashMapClass hash_map_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct HashMap),
              .__construct__ = __construct__HashMap,
              .get_size = get_size_HashMap,
              .get_len = get_len_HashMap,
              .str = str_HashMap,
              .copy = copy_HashMap,
              .insert = insert_HashMap
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
                      const void * _item) {
    struct HashMap * self = (struct HashMap *) _self;
    printf("Insertting into a hashmap\n");
    if (get_obj(_key, "Attempted to insert invalid key\n")) {
        if (get_obj(_item, NULL) ){
           const struct class_header * header = get_class_header(_key);
           if (!header->hash) {
               fprintf(stderr, "Attempt to use unhashable type as key\n");
               exit(1);
           }
           uint32_t key = header->hash(_key);
           uint32_t h = hash_internal(self->a, self->b, self->M, key);
           printf("Insering into key %d\n", h);
           struct HashItem * dest =  &self->items[key];

           while(dest->item){
                dest = dest->next; 
           }
           dest->item = _item;
           dest->key = key;
        }
    }
}

void * get_HashMap(const void * _self, 
                   const void * _key ) {
    struct HashMap * self = (struct HashMap *) _self;
    printf("getting from a hashmap\n");
    if (get_obj(_key, "Attempted to get with  invalid key\n")) {
           const struct class_header * header = get_class_header(_key);
           if (!header->hash) {
               fprintf(stderr, "Attempt to use unhashable type as key\n");
               exit(1);
           }
           uint32_t key = header->hash(_key);
           uint32_t h = hash_internal(self->a, self->b, self->M, key);
           struct HashItem * _item =  &self->items[key];
           while(_item && _item->key != key){
                _item = _item->next; 
           }
           return _item;
        }
}

const void* copy_HashMap(const void * _self){
    struct HashMap * self = (struct HashMap *) _self;
    return NULL;
}
