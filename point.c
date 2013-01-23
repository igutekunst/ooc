#include <point.h>

void * __construct__Point(void * self, va_list args);
void print_Point(void * _self);




struct PointClass point_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct Point),
              .__construct__ = __construct__Point,
              .print = print_Point
             }
};

void * Point = &point_class;



void * __construct__Point(void * _self, va_list args) {
    struct Point * self = (struct Point *) _self;
    self->class = Point;
    self->x = va_arg(args, double);
    self->y = va_arg(args, double);
    return self;
}
void print_Point(void * _self) {
    struct Point * self = (struct Point *) _self;
    printf("Point <%.2f, %.2f>\n", self->x, self->y);
}


