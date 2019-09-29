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
    struct ClassHeader* class;
    const struct String* str_repr;
    const struct String* filename;
    const struct String* mode;
    FILE* file;
    size_t size;
    size_t len;

};


struct FileClass {
    struct ClassHeader class;
};

struct FileIterator {
    struct ClassHeader* class;
    struct File* file;
    bool done;
};

struct FileIteratorClass {
    struct ClassHeader class;
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

    // Open the file
    self->file = fopen(filename, mode);
    if (self->file == NULL) {
        fprintf(stderr, "Failed to open file %s in mode %s. Error: %s\n",
                filename, mode, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Set internal strings to store filename
    self->mode = new(String, mode);
    self->filename = new(String, filename);

    // Create internal string representation for printing
    char temp_repr[200];
    snprintf(temp_repr,
             sizeof(temp_repr),
             "<open File '%s', mode '%s' at 0x%p>",
             c_str(self->filename),
             c_str(self->mode),
             self
    );

    self->str_repr = new(String, temp_repr);

    // Get the size of the file
    fseek(self->file, 0, SEEK_END);
    long file_length = ftell(self->file);
    fseek(self->file, 0, SEEK_SET);

    if (file_length < 0) {
       fprintf(stderr, "File_init:ftell error %s\n", strerror(errno));
       exit(EXIT_FAILURE);
    }

    // Length is the length of the "collection"
    // that in this case represents the number of bytes in the file

    self->len = (size_t) file_length;

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

/** @brief Return a line of text from a file
 *  allocating as much memory as necessary
 *  @return number of bytes read, or -1 on failure
 */
static ssize_t internal_File_read_line(FILE* file, char** out_line) {
    char buffer[1024] = {0};
    ssize_t line_length = 0;


    *out_line = (char*) malloc(sizeof(buffer) + 1);
    **out_line = '\0';

    while (fgets(buffer, sizeof(buffer), file) != NULL) {

        *out_line = realloc(*out_line, strlen(buffer) +
                                       strlen(*out_line) + 1);

        if (*out_line == NULL) {
            fprintf(stderr, "internal_File_read_line error: %s", strerror(errno));
            return -1;
        }

        strcat(*out_line, buffer);

        // reset buffer for reading more
        strcpy(buffer, "");
        line_length = strlen(*out_line);

        if ((*out_line)[line_length -1] == '\n') {
            break;
        }
    }
    return line_length;
}


const void* File_read_line(const void* _self) {
    struct File* self = (struct File*) _self;

    char* out_line = NULL;
    int bytes_read = internal_File_read_line(self->file, &out_line);

    if (bytes_read < 0) {
       fprintf(stderr,"File_read_line error\n");
       exit(EXIT_FAILURE);
    }
    const struct String* out = NULL;
    if (bytes_read > 0) {
        out = new(String, out_line);
    }

    free(out_line);
    return out;
}

const void* File_read(const void* _self, size_t bytes_to_read, bool read_all) {
    struct File* self = (struct File*) _self;

    if (read_all) {

        // Get bytes remaining in file
        long position = ftell(self->file);
        fseek(self->file, 0, SEEK_END);
        long bytes_remaining = ftell(self->file);
        if (bytes_remaining < 0) {
            fprintf(stderr, "File_init:ftell error %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        fseek(self->file, position, SEEK_SET);


        // Allocate temporary buffer for contents
        char* buffer = (char*) malloc(bytes_remaining);
        if (buffer == NULL) {
            fprintf(stderr, "File_read error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        fread(buffer, 1, (size_t) bytes_remaining, self->file);
        const struct String* string = new(String, buffer);
        free(buffer);
        return string;
    } else {
        char* buffer = (char*) malloc(bytes_to_read);
        if (buffer == NULL) {
            fprintf(stderr, "File_read error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        fread(buffer, 1, bytes_to_read, self->file);
        return new(String, buffer);
    }
}


const void* File_iter(const void* _self) {
    return new(FileIterator, _self);
}


const void* FileIterator_init(const void* _self, size_t argc, va_list args) {
    (void) argc;

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
                // TODO add deinit that closes file
                .get_size = File_get_size,
                .get_len = File_get_len,
                .c_str = File_to_str,
                .iter = File_iter,
                .object_name = "File",
                .file = {
                        ._file_read = File_read,
                        .file_read_line = File_read_line
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



