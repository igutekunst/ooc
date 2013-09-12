#include "object.h"

const struct class_header Class = {
   .magic = MAGIC
};

void * new (void * _class, ...) {
    struct class_header * class = (struct class_header * ) _class;
    if (!class || class->magic != MAGIC){
        exit(1);
    }
    void * new_object = (struct class_header *) malloc(class->size);
    va_list vl;
    if (class->__construct__){
        va_start(vl, _class);
        new_object = class->__construct__(new_object, vl);
        va_end(vl);
    } else {
        printf("No default constructor. Allocating %d bytes\n", class->size) ;
    }
    return new_object;
}

void * del (void * _object){
    struct class_header * class = *(struct class_header ** ) _object;
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

char *  str (void * _self){
    struct class_header * class =  * (struct class_header ** ) _self;
    if (!class || class->magic != MAGIC){
        printf("Attempted to print non object\n");
        exit(1);
    }
    if (class->to_string){
        return class->to_string(_self);
    }
    else {
        return "Object";
    }
}
void print (void * _self){
    struct class_header * class =  * (struct class_header ** ) _self;
    if (!class || class->magic != MAGIC){
        printf("Attempted to print non object\n");
        exit(1);
    }
    if (class->print)
        class->print(_self);
    else
        printf("Object at %p\n", _self);
}

size_t size(void * _self) {
    struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Attempted to get size of non object\n");
        exit(1);
    }

    if (class->get_size)
        class->get_size(_self);
    else
        return class->size;

}

void * type(void * _self){
    struct class_header * class =  * (struct class_header ** ) _self;
    if (!class || class->magic != MAGIC){
        printf("Attempt to get type of non object");
        exit(1);
    }

    return * (struct class_header **) _self;
}

size_t len(void * _self) {
    struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Attempted to get size of non object\n");
        exit(1);
    }

    if (class->get_len) 
        return class->get_len(_self);
    else {
        printf("TypeError: object does not suppport len\n") ;
        exit(1);
    }

}

void * append(void * _self, void * _other){
    struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Attempted to append non object\n");
        exit(1);
    }

    if(class->append)
        class->append(_self, _other);
    else
        printf("Type does not support append\n");
}

void * play(void * _self){
    struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Attempted to append non object\n");
        exit(1);
    }

    if(class->play)
        class->play(_self);
    else
        printf("Type does not support append\n");
}
