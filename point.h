#ifndef POINT_H
#define POINT_H
#include <object.h>
#include <string.h>

struct Point{
    struct class_header class;
    double x;
    double y;
};
extern void * Point;

void print(void * object);
#endif
