#include <assert.h>
#include <ooc/object.h>
#include <ooc/string.h>
#include "object_internal.h"

const struct class_header Class = {
   .magic = MAGIC
};

/**
 *
 * @param _class
 * @param ...
 * @return
 */
const void * new (const void * const _class, ...) {
    const struct class_header * class = (struct class_header * ) _class;
    if (!class || class->magic != MAGIC){
        fprintf(stderr, "new called with invalid class\n");
        exit(1);
    }
    struct ObjectHeader*  new_object = (struct ObjectHeader *) malloc(class->size);
    va_list vl;
    if (class->object_init){
        va_start(vl, _class);
        //TODO remove extra initialization from specific class constructors
        new_object->class = class;
        new_object = class->object_init(new_object, vl);
        va_end(vl);
    } else {
        printf("No default constructor. Allocating %zu bytes\n", class->size) ;
    }
    return new_object;
}


void del (const void *_object){
    const struct class_header * class;
    if ((class = get_class_header_msg(_object, "Attempt to delete non object\n"))) {
        if( class->object_deinit) {
            class->object_deinit(_object);
        } else {
            free((void*) _object) ;
        }
    }
}


const char *  str (const void * _self){
    const struct class_header * class;
    if ((class = get_class_header_msg(_self, "str called on non object\n"))) {
        if (class->str){
            return class->str(_self);
        }
        else
            return class->object_name;
    }
    return NULL;
}

const void *to_String(const void *_object) {
    return new(String, str(_object));

}


void print (const void * _self){
    const struct class_header * class;
    if ((class = get_class_header_msg(_self, "Attempted to print non object\n"))) {
        if (class->print) {
            class->print(_self);
        } else if (class->str) {
            printf("%s", class->str(_self));
        } else
        printf("Object at %p\n", _self);
    }
}


size_t size(const void * _self) {
    const struct class_header * class;
    if ((class = get_class_header_msg(_self, "Attempted to get type of non object\n"))) {
        if (class->get_size)
            return class->get_size(_self);
        return class->size;
    }
    return 0;
}


const void * type(const void * _self){
    const struct class_header * class;
    if ((class = get_class_header_msg(_self, "Attempted to get type of non object\n")))
        return * (struct class_header **) _self;
    return NULL;
}


size_t len(const void * _self) {
    const struct class_header * class;
    if ((class = get_class_header_msg(_self, "Attempted to get len of non object\n"))) {
        if (class->get_len) 
            return class->get_len(_self);
        else {
            fprintf(stderr, "TypeError: %s does not support len\n", clsname(_self));
            exit(1);
        }
    }
    return 0;
}


const char* clsname(const void * _self) {
    const struct class_header * class;
    if ((class = get_class_header_msg(_self, "Attempted to get name of invalid object\n"))) {
        if (class->object_name)
            return class->object_name;
        else {
            printf("TypeError: object does not support name\n.") ;
            exit(1);
        }
    }
    return 0;
}




const void * copy(const void * _self) {
    const struct class_header * class;
    if ((class = get_class_header_msg(_self, "Attempted to get len of non object\n"))){
        if (class->copy) 
            return class->copy(_self);
        else {
            printf("TypeError: object does not support copying\n") ;
            exit(1);
        }
    }
    return NULL;
}


bool equals(const void * _self, const void * _other){
    const struct class_header * class;
    const struct class_header * other;

    if ((class = get_class_header_msg(_self, "attempted to append non object\n"))) {
        other = get_class_header_msg(_other, "attempted to compare non object\n");
        if(class->equals){
            return class->equals(_self, _other);
        }  else {
            return _self == _other;
        }
    }
    return NULL;
}

const void* append(const void * _self, const void * _other){
    const struct class_header * class;

    if ((class = get_class_header_msg(_self, "Attempted to append non object\n"))) {
        if(class->append) {
            return class->append(_self, _other);
        } else {
            fprintf(stderr, "Type %s does not support append\n", clsname(_self));
            exit(EXIT_FAILURE);
        }
    }
}


inline struct class_header * get_class_header_msg(const void * _self, const char * message){
    if (_self == NULL) {
        fprintf(stderr, "get_class_header_msg called with NULL object. Message: %s\n", message);
        exit(1);
    }
    struct class_header * class =  * (struct class_header ** ) _self;
    if (class && class->magic == MAGIC){
        return class;
    }
    if (message) {
        fprintf(stderr, "%s", message);
        exit(1);
    } else {
        fprintf(stderr, "get_class_header_msg failed for unknown reason");
        exit(1);
    }
}


inline struct class_header * get_obj_type(const void * _self, const void* class, const char * message ){
    struct class_header * class_header =  * (struct class_header ** ) _self;
    if (class_header && class_header->magic == MAGIC){
        if (class_header == class) {
            return class;
        }
    }
    if (message) {
        fprintf(stderr, "%s", message);
        exit(1);
    } else {
        fprintf(stderr, "get_class_header_msg failed for unknown reason");
        exit(1);
    }

}


inline const struct class_header * get_class_header(const void * _self){
    return   * (struct class_header ** ) _self;
}

uint64_t hash(const void * _self) {
    
    if(get_class_header_msg(_self, "hash called with invalid object")){
        const struct class_header * self = get_class_header(_self);
        return self->hash(_self) ;
    }
    return 0;
}

void * insert(const void * _self, 
                      const void * _key, 
                      const void * _other) {
    
    if(get_class_header_msg(_self, "Failed to insert into non collection\n")){
        const struct class_header * self = get_class_header(_self);
        assert(self->insert);
            self->insert(_self, _key, _other) ;
    }
    return NULL;
}

void del_item(const void * _self, const void * _key) {
    if(get_class_header_msg(_self, "Failed to get from non collection\n")){
        const struct class_header * self = get_class_header(_self);
        if (self->del_item) {
            self->del_item(_self, _key);
        } else {
            printf("TypeError: object does not support del. \n") ;
            exit(1);
        }
    }
    return ;
}

const void * get_item(const void * _self, const void * _key ) {
    if(get_class_header_msg(_self, "get_item called on invalid object")){
        const struct class_header * self = get_class_header(_self);
        if (self->get_item == NULL) {
            fprintf(stderr, "%s does not support get_item\n", clsname(_self));
            exit(EXIT_FAILURE);
        }
        return self->get_item(_self, _key) ;
    }
    return NULL;
}

const void * iter(const void * _self ) {
    if(get_class_header_msg(_self, "Failed to create iterator from non collection\n")){
        const struct class_header * self = get_class_header(_self);
        if(self->iter == NULL ) {
            fprintf(stderr, "Class %s does not support iteration.\n", clsname(_self));
            exit(EXIT_FAILURE);

        }
        return self->iter(_self) ;
    }
    return NULL;
}

const void * next(const void * _self ) {
    if(get_class_header_msg(_self, "Failed to get next on non object\n")){
        const struct class_header * self = get_class_header(_self);
        if(self->next){
            return self->next(_self) ;
        }
    }
    return NULL;
}
