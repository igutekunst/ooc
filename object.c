#include <object.h>

const struct class_header Class = {
   .magic = MAGIC
};

const void * new (const void * const _class, ...) {
    const struct class_header * class = (struct class_header * ) _class;
    if (!class || class->magic != MAGIC){
        exit(1);
    }
    const void * new_object = (struct class_header *) malloc(class->size);
    va_list vl;
    if (class->__construct__){
        va_start(vl, _class);
        new_object = class->__construct__(new_object, vl);
        va_end(vl);
    } else {
        printf("No default constructor. Allocating %zu bytes\n", class->size) ;
    }
    return new_object;
}

void * del (void * _object){
    const struct class_header * class = *(struct class_header ** ) _object;
    if (!class || class->magic != MAGIC){
        printf("Attempt to delete non object\n");
        exit(1);
    }
    if( class->__destruct__) {
        _object = class->__destruct__(_object);
    } else {
        printf("No destructor. Freeing\n");
        free(_object) ;
    }
    
    return _object;
}

const char *  str (const void * _self){
    const struct class_header * class =  * (struct class_header ** ) _self;
    if (!class || class->magic != MAGIC){
        printf("Attempted to print non object\n");
        exit(1);
    }
    if (class->str){
        return class->str(_self);
    }
    else {
        return "Object";
    }
}
void print (const void * _self){
    const struct class_header * class =  * (struct class_header ** ) _self;
    if (!class || class->magic != MAGIC){
        printf("Attempted to print non object\n");
        exit(1);
    }
    if (class->print) {
        class->print(_self);
    } else if (class->str) {
        printf("%s", class->str(_self));
    }
    else
        printf("Object at %p\n", _self);
}

size_t size(const void * _self) {
    const struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Attempted to get size of non object\n");
        exit(1);
    }

    if (class->get_size)
        return class->get_size(_self);
    return class->size;

}

const void * type(const void * _self){
    const struct class_header * class =  * (struct class_header ** ) _self;
    if (!class || class->magic != MAGIC){
        printf("Attempt to get type of non object");
        exit(1);
    }

    return * (struct class_header **) _self;
}

size_t len(const void * _self) {
    const struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Attempted to get len of non object\n");
        exit(1);
    }

    if (class->get_len) 
        return class->get_len(_self);
    else {
        printf("TypeError: object does not suppport len\n") ;
        exit(1);
    }

}
const void * copy(const void * _self) {
    const struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Attempted to get len of non object\n");
        exit(1);
    }

    if (class->copy) 
        return class->copy(_self);
    else {
        printf("TypeError: object does not suppport copying\n") ;
        exit(1);
    }

}

const void * append(const void * _self, const void * _other){
    const struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Attempted to append non object\n");
        exit(1);
    }

    if(class->append)
        return class->append(_self, _other);

    else {
        fprintf(stderr, "Type does not support append\n");
        exit(EXIT_FAILURE);
    }
}

void  play(void * _self){
    const struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Attempted to append non object\n");
        exit(1);
    }

    if(class->play)
        class->play(_self);
    else{
        fprintf(stderr, "Type does not support append\n");
        exit(EXIT_FAILURE);
    }
}



inline bool get_obj(const void * _self, const char * message){
    const struct class_header * class =  * (struct class_header ** ) _self;
    if (class && class->magic == MAGIC){
        return true;
    }
    if (message) {
        printf("%s", message);
        exit(1);
    } else {
        return false; 
    }

}


inline const struct class_header * get_class_header(const void * _self){
    return   * (struct class_header ** ) _self;
}

uint32_t hash(const void * _self) {
    
    if(get_obj(_self, "Failed to insert into non collection\n")){
        const struct class_header * self = get_class_header(_self);
        return self->hash(_self) ;
    }

}

void * insert(const void * _self, 
                      const void * _key, 
                      const void * _other) {
    
    if(get_obj(_self, "Failed to insert into non collection\n")){
        const struct class_header * self = get_class_header(_self);
        if (self->insert)
            self->insert(_self, _key, _other) ;
    }

}

void * get(const void * _self, 
                      const void * _key ) {
    if(get_obj(_self, "Failed to get from non collection\n")){
        const struct class_header * self = get_class_header(_self);
        if(self->get)
            return self->get(_self, _key) ;
    }

}
