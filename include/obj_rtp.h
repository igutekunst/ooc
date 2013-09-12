#ifndef RTP_H
#define RTP_H

#include <object.h>
#include <string.h>
#include <obj_string.h>
#include <bus.h>
#include <stdbool.h>
struct RtpClass{
    struct class_header class;
};
struct Rtp{
    struct class_header * class;
    struct String * object;
    size_t size;
    Proxy proxy;
};
extern void * Rtp;

#endif
