//
// Created by Isaac Gutekunst on 2019-09-28.
//

#ifndef OOC_FILE_INTERNAL_H
#define OOC_FILE_INTERNAL_H
#include <ooc/object_va_args.h>

// Actual function
const void* _file_read(size_t argc, ...);

/**
 * @brief read a file
 *
 * Reads up [size] bytes, and return a ooc String object.
 * If no size is specified, read until EOF is reached.
 *
 * The file will keep track of the internal seek position, so
 * repeated calls will read further into the file.
 *
 * @param file ooc File object
 * @param ... Optional size in bytes to read
 * @return ooc String object
 */
#define file_read(...) _file_read(PP_NARG(__VA_ARGS__), __VA_ARGS__)

/**
 * @brief read one line from a file
 * allocating as much memory as needed. Returns a new ooc String
 * object, or NULL if no bytes are read.
 * @param _file
 * @return ooc File object, or NULL
 */
const void* file_read_line(const void* _file);

extern void * FileIterator;
extern void * File;

#endif //OOC_FILE_H
