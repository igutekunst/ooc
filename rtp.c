#include <obj_rtp.h>

void * __construct__Rtp(void * self, va_list args);
void print_Rtp(void * _self);
size_t get_size_Rtp(void * _self);
bool play_Rtp(void * _self);

struct RtpClass rtp_class = {
    .class = {.magic = MAGIC,
              .size = sizeof(struct Rtp),
              .__construct__ = __construct__Rtp,
              .print = print_Rtp,
              .get_size = get_size_Rtp,
              .play = play_Rtp
             }

};

void * Rtp = &rtp_class;


size_t get_size_Rtp(void * _self){
    struct Rtp * self = (struct Rtp *) _self;
    return self->size;
}

void * __construct__Rtp(void * _self, va_list args) {
    struct Rtp * self = (struct Rtp *) _self;
    char * data = va_arg(args, char *);
    self->size = sizeof(struct Rtp) ;
    printf("rtp object being made. \n");
    self->object = new(String, data);
    Proxy p = {
        .remote_path    = "com.axis.Streamer",
        .interface      = "com.axis.Streamer.RTP.Pipeline",
        .object = str(self->object)
    };
    printf("to_str in constructor %s", str(self->object));
    self->proxy = p;
    // Initialize components for dbus call
    
    self->class = Rtp;
    return self;
}
void print_Rtp(void * _self) {
    struct Rtp * self = (struct Rtp *) _self;
    printf("Printing object seperately\n");
    printf("<RTP Object: %s >", str(self->object));
}

bool play_Rtp(void * _self) {
    struct Rtp * self = (struct Rtp *) _self;
}




