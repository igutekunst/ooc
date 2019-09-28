//
// Created by Isaac Gutekunst on 2019-09-28.
//

#include <ooc/file.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ooc/string.h>
#include <ooc/math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ooc/object.h>
#include <string.h>
#include <errno.h>

#include "ooc/list.h"
#include "object_internal.h"


struct File {
    struct class_header* class;
    const struct String* str_repr;
    const struct String* filename;
    const struct String* mode;
    FILE* file;
    size_t size;
    size_t len;

};


struct FileClass {
    struct class_header class;
};

struct FileIterator {
    struct class_header* class;
    struct File* file;
    bool done;
};

struct FileIteratorClass {
    struct class_header class;
};


const void* File_init(const void* _self, size_t argc, va_list args) {
    struct File* self = (struct File*) _self;

    self->len = 0;
    self->size = sizeof(struct File);

    if (argc == 0 || argc > 2) {
        fprintf(stderr, "File arguments filename, [mode]\n");
    }

    const char* mode;
    if (argc == 1) {
        mode = "r";
    } else {
        mode = va_arg(args, const char*);
    }

    const char* filename = va_arg(args, const char*);

    self->mode = new(String, mode);
    self->filename = new(String, filename);

    self->file = fopen(filename, mode);
    if (self->file == NULL) {
        fprintf(stderr, "Failed to open file %s in mode %s. Error: %s",
                filename, mode, strerror(errno));
        exit(EXIT_FAILURE);
    }
    char temp_repr[200];
    snprintf(temp_repr,
             sizeof(temp_repr),
             "<open File '%s', mode '%s' at 0x%p>",
             c_str(self->filename),
             c_str(self->mode),
             self
    );

    self->str_repr = new(String, temp_repr);

    return _self;
}


size_t File_get_size(const void* _self) {
    struct File* self = (struct File*) _self;
    return self->size;
}

size_t File_get_len(const void* _self) {
    struct File* self = (struct File*) _self;
    return self->len;
}

const char* File_to_str(const void* _self) {
    struct File* self = (struct File*) _self;
    return c_str(self->str_repr);
}

const void* File_read(const void* _self, size_t bytes_to_read, bool read_all) {
    struct File* self = (struct File*) _self;

    if (read_all) {
        fprintf(stderr, "Reading whole file not supported yet\n");
        exit(EXIT_FAILURE);
    } else {
        char* buffer = (char*) malloc(bytes_to_read);
        fread(buffer, 1, bytes_to_read, self->file);
        return new(String, buffer);
    }

    // shouldn't get here
    assert(false);
}


const void* File_iter(const void* _self) {
    return new(FileIterator, _self);
}


const void* FileIterator_init(const void* _self, size_t argc, va_list args) {
    struct FileIterator* self = (struct FileIterator*) _self;

    self->file = (struct File*) va_arg(args, const void *);
    assert(self->file);

    self->done = false;


    return self;
}


const void* FileIterator_next(const void* _self) {
    struct FileIterator* self = (struct FileIterator*) _self;
    return NULL;
}


struct FileClass File_class = {
        .class = {
                .magic = MAGIC,
                .size = sizeof(struct File),
                .object_init = File_init,
                .get_size = File_get_size,
                .get_len = File_get_len,
                .c_str = File_to_str,
                .iter = File_iter,
                .object_name = "File",
                .file = {
                        .file_read = File_read
                }

        }
};

struct FileIteratorClass file_iterator_class = {
        .class = {
                .magic = MAGIC,
                .object_init = FileIterator_init,
                .size = sizeof(struct FileIterator),
                .next = FileIterator_next
        }

};

void* FileIterator = &file_iterator_class;

void* File = &File_class;



