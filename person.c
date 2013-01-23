#include <person.h>



void * __construct__Person(void * self, va_list args);
void * __destruct__Person(void * self, va_list args);
void print_Person(void * _self);



struct Person person_class = {
    .class = {.magic = MAGIC, 
              .size  = (sizeof(struct Person)),
              .print = print_Person,
              .__construct__ = __construct__Person,
             }
};

void * Person = &person_class;


//TODO make maxlength values on names..
//TODO determine whether to cash string length (probably yes)
void * __construct__Person(void * _self, va_list args) {
    struct Person * self = (struct Person *) _self;
    char * first = va_arg(args, char *);
    char * last =  va_arg(args, char *);
    
    self = (struct Person *) realloc(self, sizeof(struct Person)    + 
                         strlen(first) + 1 +
                         strlen(last)  + 1);
    void * space = (void * ) ( &self[1]);
    self->first  = strcpy(space, first);
    self->last   = strcpy(space + strlen(self->first)+1, last);

    self->age = va_arg(args, unsigned int);

    return self;
}
void print_Person(void * _self) {
    struct Person * self = (struct Person *) _self;
    printf("Person <%s %s. Age: %d>\n", self->first, self->last, self->age);
}

void print (void * _self){
    struct class_header * class =  * (struct class_header ** ) _self;

    if (!class || class->magic != MAGIC){
        printf("Invalid object passed to print\n");
        exit(1);
    }
    if (class->print)
        class->print(_self);
    else
        printf("Object at %p\n", _self);
}

