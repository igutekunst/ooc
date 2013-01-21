#include <point.h>

void * __construct__Point(void * self, va_list args);
void print_Point(void * _self);



struct Point point_class = {
    .class = {.magic = MAGIC, 
              .size  = (sizeof(struct Point)),
              .print = print_Point,
              .__construct__ = __construct__Point
             }
};

void * Point = &point_class;



void * __construct__Point(void * _self, va_list args) {
    struct Point * point = (struct Point *) _self;
    point->x = va_arg(args, double);
    point->y = va_arg(args, double);
    return _self;
}
void print_Point(void * _self) {
    struct Point * self = (struct Point *) _self;
    printf("Point <%.2f, %.2f>\n", self->x, self->y);
}


