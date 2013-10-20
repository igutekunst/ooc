#include "point.h"

const void * __construct__Point(const void * const self, va_list args);
void print_Point(const void * _self);
size_t get_size_Point(const void * _self);



struct PointClass point_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct Point),
              .__construct__ = __construct__Point,
              .print = print_Point,
              .get_size = get_size_Point,
              .get_len = 0
             }

};

void * Point = &point_class;


size_t get_size_Point(const void * _self){
    struct Point * self = (struct Point *) _self;
    return self->size;
}


const void * __construct__Point(const void * _self, va_list args) {
    struct Point * self = (struct Point *) _self;
    self->class = Point;
    self->size = sizeof(struct Point);
    self->x = va_arg(args, double);
    self->y = va_arg(args, double);
    return self;
}


void print_Point(const void * _self) {
    struct Point * self = (struct Point *) _self;
    printf("Point <%.2f, %.2f>\n", self->x, self->y);
}


