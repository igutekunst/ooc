#ifndef OOC_STRING_H
#define OOC_STRING_H
#include <ooc/object_va_args.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

extern void * String;

const void* _string_split(size_t argc, ...);

/**
 * @brief split a string into pieces
 *
 * Splits a string on an optional substring. Defaults to space
 *
 * @param _string ooc String class
 * @param substring to split on.
 * @return ooc List object of ooc String objects
 */

#define string_split(...) _string_split(PP_NARG(__VA_ARGS__), __VA_ARGS__)

const void* string_slice (const void* _string, ssize_t start, ssize_t end);

#endif
