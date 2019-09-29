//
// Created by Isaac Gutekunst on 2019-09-28.
//

#include "trait_file.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ooc/object.h>
#include <ooc/file.h>
#include "object_internal.h"

const void* _file_read(size_t argc, ...) {
    if (argc == 0 || argc > 2) {
        fprintf(stderr, "_file read takes 1 or two arguments\n");
        exit(EXIT_FAILURE);
    }
    va_list args;
    va_start(args, argc);
    const void * _file = va_arg(args, const void*);

    struct class_header* file_class_header = get_class_header_msg(_file, "_file_read: invalid argument\n");
    size_t bytes_to_read = 0;
    if (file_class_header->file._file_read == NULL) {
        fprintf(stderr, "_file_read: %s does not support file_read\n", class_name(_file));
        exit(EXIT_FAILURE);
    }

    bool read_all = true;
    if (argc == 2) {
        bytes_to_read = va_arg(args, size_t);
        read_all = false;
        va_end(args);
        return file_class_header->file._file_read(_file, bytes_to_read, read_all);
    } else {
        va_end(args);
        return file_class_header->file._file_read(_file, bytes_to_read, read_all);
    }
}

const void* file_read_line(const void* _file) {
    struct class_header* file_class_header = get_class_header_msg(_file, "_file_read_line: invalid argument\n");
    if (file_class_header->file._file_read == NULL) {
        fprintf(stderr, "_file_read_line: %s does not support file_read\n", class_name(_file));
        exit(EXIT_FAILURE);
    }
    return file_class_header->file.file_read_line(_file);

}

