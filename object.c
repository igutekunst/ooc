#include "object.h"


const struct class_header Class = {
   .magic = MAGIC
};

void * new (void * _class, ...) {
    struct class_header * class = (struct class_header * ) _class;
    if (!class || class->magic != MAGIC){
        printf("shitty dog\n");
        exit(1);
    }
    void * new_object = (struct class_header *) malloc(class->size);
    printf("Allocating %d bytes\n", class->size) ;
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

void * delete (void * _object){
    struct class_header * class = (struct class_header * ) _object;
    if (!class || class->magic != MAGIC){
        printf("shitty dog\n");
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









































































