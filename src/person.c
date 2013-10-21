#include "person.h"



const void * __construct__Person(const void * self, va_list args);
void * __destruct__Person(const void * self, va_list args);
void print_Person(const void * _self);
size_t get_size_Person(const void * _self);


struct PersonClass{
    struct class_header class;
};
struct PersonClass person_class = {
    .class = {.magic = MAGIC, 
              .size  = (sizeof(struct Person)),
              .print = print_Person,
              .get_size = get_size_Person,
              .__construct__ = __construct__Person,
             }
};

void * Person = &person_class;


//TODO make maxlength values on names..
//TODO determine whether to cash string length (probably yes)
const void * __construct__Person(const void * _self, va_list args) {
    struct Person * self = (struct Person *) _self;
    char * first = va_arg(args, char *);
    char * last =  va_arg(args, char *);
    size_t size =  sizeof(struct Person)    + 
                 strlen(first) + 1 +
                 strlen(last)  + 1;
    self = (struct Person *) realloc(self, size);
    self->size = size;
    self->class = Person;
    void * space = (void * ) ( &self[1]);
    self->first  = strcpy(space, first);
    self->last   = strcpy(space + strlen(self->first)+1, last);

    self->age = va_arg(args, unsigned int);

    return self;
}

size_t get_size_Person(const void * _self){
    struct Person * self = (struct Person *) _self;
    return self->size;
}

void print_Person(const void * _self) {
    struct Person * self = (struct Person *) _self;
    printf("Person <%s %s. Age: %d>\n", self->first, self->last, self->age);
}


