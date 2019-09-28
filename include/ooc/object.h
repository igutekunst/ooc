#ifndef OBJECT_H
#define OBJECT_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <ooc/object_va_args.h>

/**
 * @defgroup common Common functions
 * @{
 */

/**
 * @brief Create a new ooc object.
 *
 * This function allocates memory and returns a void* to a
 * newly allocated ooc object, or NULL on failure.
 *
 * _class should be a pointer to an ooc class object, such as
 * String or HashMap.
 * @param class ooc class object
 * @param argc number of arguments provided
 * @return new object or type class, or NULL
 */
const void* _new(size_t argc, ...);

/**
 * @brief get ooc class object
 * @param self ooc object
 * @return ooc class of a given ooc object
 */
const void* type(const void* self);

/**
 * @brief delete an ooc object
 * @param object
 */
void del(const void* object);

/**
 * @brief get size in bytes
 *
 * Get the full size in bytes of an ooc object.
 * This is implementation defined, and may not include the
 * size of items contained in a collection for example
 * @param _object
 * @return size in bytes of object
 */
size_t size(const void* _object);

const char* clsname(const void* _self);

void print(const void* _object);

size_t len(const void* _object);

const void* copy(const void* _object);

bool equals(const void* _object, const void* _other);

typedef enum {
    COMPARE_GT,
    COMPARE_EQ,
    COMPARE_LT
} CompareValue;

CompareValue compare(const void* _self, const void* _other);

/**
 * @brief Sort a collection in place
 * @param _self
 */
void obj_sort(const void* object);

const void* sorted(const void* _self);


/**  @}
 * End of Common functions
 * */


const char* c_str(const void* _object);

const char* c_str_repr(const void* _object);

const void* to_String(const void* _object);


/**
 *
 * @defgroup collection Collections
 * @{
 */
const void* append(const void* _object, const void* _other);

void* set_item(const void* _self,
               const void* _key,
               const void* item);

const void* get_item(const void* _self,
                     const void* _key);

struct class_header* get_class_header_msg(const void* _self, const char* message);

inline const struct class_header* get_obj_type(const void* _self, const void* class, const char* message);

const void* iter(const void* _object);

uint64_t hash(const void* _self);

const void* next(const void* _self);

void del_item(const void* _self, const void* key);

/**
 * @brief Create an iterator for an collection's keys
 *
 * If object is a collection supporting a map of
 * keys to values, such as a list or HashMap,
 * return an iterator that iterates over the keys.
 *
 * The order of iteration is undefined any may change between
 * calls.
 *
 * For a HashMap, this is the same as iter(hash_map), modelled
 * after python.
 * ```
 *  foo = {'a' : 5, 'b' 2}
 *  for f in foo:
 *      print f
 *
 *  'a'
 *  'b'
 *  ```
 *
 * @param object Object supporting key:value pair mapping
 */
void keys(const void* object);

/**
 * @brief Create an iterator for an collection's values
 *
 * If object is a collection supporting a map of
 * keys to values, such as a list or HashMap,
 * return an iterator that iterates over the values
 *
 * The order of iteration is undefined any may change between
 * calls.
 *
 * @param object Object supporting key:value pair mapping
 */
void values(const void* _self);

/** @}
 */



extern const struct class_header Class;


const struct class_header* get_class_header(const void* _self);


#define new(...) _new(PP_NARG(__VA_ARGS__), __VA_ARGS__)

#define typed_new(class_name, ...) \
    (struct class_name*) new(class_name, __VA_ARGS__)

#endif
