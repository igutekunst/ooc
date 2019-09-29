//
// Created by Isaac Gutekunst on 2019-09-29.
//

#include <stdio.h>
#include "trait_string.h"
#include "object_internal.h"

const void* _string_split(size_t argc, ... ) {
    if (argc == 0 || argc > 2) {
        fprintf(stderr, "_string_split takes one or two arguments\n");
        exit(EXIT_FAILURE);
    }

    const void* _sep = NULL;

    va_list args;
    va_start(args, argc);
    const void * _string = va_arg(args, const void*);
    if (argc == 2) {
       _sep = va_arg(args, const void*);
    }

    va_end(args);

    struct class_header* file_class_header = get_class_header_msg(_string, "string_split: invalid argument\n");
    if (file_class_header->string.string_split == NULL) {
        fprintf(stderr, "string_split: %s does not support string_split\n", class_name(_string));
        exit(EXIT_FAILURE);
    }

    return file_class_header->string.string_split(_string, _sep);

}


const void* string_slice (const void* _string, ssize_t start, ssize_t end) {
    struct class_header* file_class_header = get_class_header_msg(_string, "string_split: invalid argument\n");
    if (file_class_header->string.string_slice == NULL) {
        fprintf(stderr, "string_slice: %s does not support string_slice\n", class_name(_string));
        exit(EXIT_FAILURE);
    }

    return file_class_header->string.string_slice(_string, start, end);

}
