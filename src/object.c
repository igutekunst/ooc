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


void * del (void * _self){
    const struct class_header * class;
    if ((class = get_obj(_self,"Attempt to delete non object\n"))) {
        if( class->__destruct__) {
            _self = class->__destruct__(_self);
        } else {
            free(_self) ;
            return NULL;
        }
        return _self;
    }
    return NULL;
}


const char *  str (const void * _self){
    const struct class_header * class;
    if ((class = get_obj(_self,"Attempted to print non object\n" ))) {
        if (class->str){
            return class->str(_self);
        }
        else
            return "Object";
    }
    return NULL;
}


void print (const void * _self){
    const struct class_header * class;
    if ((class = get_obj(_self,"Attempted to print non object\n" ))) {
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
    if ((class = get_obj(_self,"Attempted to get type of non object\n" ))) {
        if (class->get_size)
            return class->get_size(_self);
        return class->size;
    }
    return 0;
}


const void * type(const void * _self){
    const struct class_header * class;
    if ((class = get_obj(_self,"Attempted to get type of non object\n" )))
        return * (struct class_header **) _self;
    return NULL;
}

size_t len(const void * _self) {
    const struct class_header * class;
    if ((class = get_obj(_self,"Attempted to get len of non object\n" ))) {
        if (class->get_len) 
            return class->get_len(_self);
        else {
            printf("TypeError: object does not suppport len\n") ;
            exit(1);
        }
    }
    return 0;
}

void del_item(const void * _self, const void * key) {
    const struct class_header * class;
    if ((class = get_obj(_self,"Attempted to call del_item on non object\n" ))) {
        if (class->del_item) 
            class->del_item(class, key);
        else {
            printf("TypeError: object does not suppport deleting items. \n") ;
            exit(1);
        }
    }
    return ;
}


const void * copy(const void * _self) {
    const struct class_header * class;
    if ((class = get_obj(_self,"Attempted to get len of non object\n"))){
        if (class->copy) 
            return class->copy(_self);
        else {
            printf("TypeError: object does not suppport copying\n") ;
            exit(1);
        }
    }
    return NULL;
}


bool equals(const void * _self, const void * _other){
    const struct class_header * class;
    const struct class_header * other;

    if ((class = get_obj(_self,"attempted to append non object\n" ))) {
        other = get_obj(_other,"attempted to compare non object\n" );
        if(class->equals){
            return class->equals(_self, _other);
        } else  if (class->hash && other->hash) {
            return hash(_self) == hash(_other);
        } else{
            fprintf(stderr, "type does not support comparrason\n");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

const void * append(const void * _self, const void * _other){
    const struct class_header * class;

    if ((class = get_obj(_self,"attempted to append non object\n" ))) {
        if(class->append)
            return class->append(_self, _other);
        else {
            fprintf(stderr, "type does not support append\n");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

void  play(void * _self){
    const struct class_header * class;
    if ((class = get_obj(_self, "Attempted to append non object\n"))) {
        if(class->play)
            class->play(_self);
        else{
            fprintf(stderr, "Type does not support append\n");
            exit(EXIT_FAILURE);
        }
    }
}



inline struct class_header * get_obj(const void * _self, const char * message){
    struct class_header * class =  * (struct class_header ** ) _self;
    if (class && class->magic == MAGIC){
        return class;
    }
    if (message) {
        printf("%s", message);
        exit(1);
    } else {
        return NULL;
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
    return 0;
}

void * insert(const void * _self, 
                      const void * _key, 
                      const void * _other) {
    
    if(get_obj(_self, "Failed to insert into non collection\n")){
        const struct class_header * self = get_class_header(_self);
        if (self->insert)
            self->insert(_self, _key, _other) ;
    }
    return NULL;
}

void * get_item(const void * _self, 
                      const void * _key ) {
    if(get_obj(_self, "Failed to get from non collection\n")){
        const struct class_header * self = get_class_header(_self);
        if(self->get)
            return self->get(_self, _key) ;
    }
    return NULL;
}

const void * iter(const void * _self ) {
    if(get_obj(_self, "Failed to get from non collection\n")){
        const struct class_header * self = get_class_header(_self);
        if(self->iter)
            return self->iter(_self) ;
    }
    return NULL;
}

const void * next(const void * _self ) {
    if(get_obj(_self, "Failed to get next on non object\n")){
        const struct class_header * self = get_class_header(_self);
        if(self->next){
            return self->next(_self) ;
        }
    }
    return NULL;
}
