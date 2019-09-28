#ifndef OBJECT_H
#define OBJECT_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

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
 * @return new object or type class, or NULL
 */
const void *new(const void *class, ...);

/**
 * @brief get ooc class object
 * @param self ooc object
 * @return ooc class of a given ooc object
 */
const void *type(const void *self);

/**
 * @brief delete an ooc object
 * @param object
 */
void del(const void *object);

/**
 * @brief get size in bytes
 *
 * Get the full size in bytes of an ooc object.
 * This is implementation defined, and may not include the
 * size of items contained in a collection for example
 * @param _object
 * @return size in bytes of object
 */
size_t size(const void *_object);

const char *clsname(const void *_self);

void print(const void *_object);

size_t len(const void *_object);

const void *copy(const void *_object);

bool equals(const void *_object, const void *_other);

/**  @}
 * End of Common functions
 * */


const char *str(const void *_object);

const void *to_String(const void *_object);


/**
 *
 * @defgroup collection Functions for working with collections
 * @{
 */
const void *append(const void *_object, const void *_other);

void *insert(const void *_self,
             const void *_key,
             const void *item);

const void *get_item(const void *_self,
                     const void *_key);

struct class_header *get_class_header_msg(const void *_self, const char *message);

inline struct class_header *get_obj_type(const void *_self, const void *class, const char *message);

const void *iter(const void *_object);

uint64_t hash(const void *_self);

const void *next(const void *_self);

void del_item(const void *_self, const void *key);

/**
 * @}
 */


extern const struct class_header Class;


const struct class_header *get_class_header(const void *_self);

#define typed_new(class_name, ...) \
    (struct class_name*) new(class_name, __VA_ARGS__)

#endif
