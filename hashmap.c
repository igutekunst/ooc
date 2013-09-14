#include <obj_hashmap.h>


void * __construct__HashMap(void * self, va_list args);
void print_HashMap(void * _self);
size_t get_size_HashMap(const void * _self);
size_t get_len_HashMap(const void * _self);
const char * str_HashMap(const void *_self);
void * insert_HashMap(const void * _self, 
                      const void * key, 
                      const void * _other);
const void * copy_HashMap(const void * _self);

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

void * __construct__HashMap(void * _self, va_list args) {
    struct HashMap * self = (struct HashMap *) _self;
    //const char * data = va_arg(args, const char *);
    // for now only support empty constructor
    size_t size = sizeof(struct HashMap );
    self->len = 0;
    return self;
}
void print_HashMap(void * _self) {
    struct HashMap * self = (struct HashMap *) _self;
    printf("%s", self->data);
}


const char * str_HashMap(const void * _self){
    struct HashMap * self = (struct HashMap *) _self;
    return self->data;
}
void * insert_HashMap(const void * _self, 
                      const void * _key, 
                      const void * _item) {
    struct HashMap * self = (struct HashMap *) _self;
    if (is_obj(_item, NULL)) {
        if (is_obj(_key, NULL) ){
           const struct class_header * header = get_class_header(_key);
           if (!header->hash) {
               fprintf(stderr, "Attempt to use unhashable type as key\n");
               exit(1);
           }
           uint32_t key = header->hash(_key);
        }
    }
}

const void* copy_HashMap(const void * _self){
    struct HashMap * self = (struct HashMap *) _self;
    return new(HashMap, self->data);
}
