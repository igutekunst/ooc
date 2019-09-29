#ifndef OBJECT_H
#define OBJECT_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <ooc/object_va_args.h>


const void* _new(size_t argc, ...);
/**
 * @brief Create a new ooc object.
 *
 * This function allocates memory and returns a void* to a
 * newly allocated ooc object, or NULL on failure.
 *
 * The first argument, class, is an OOC "Class", and
 * is exposed to the user layer as an extern void*. New
 * is able to unwrap the void*, and call the appropriate constructor
 * for a type, and allocate a new OOC object. On failure, new will
 * exit the program and print an error message.
 *
 * @param class ooc class object
 * @param argc number of arguments provided
 * @return new object or type class, or NULL
 */
#define new(...) _new(PP_NARG(__VA_ARGS__), __VA_ARGS__)

/**
 * @brief get ooc class object
 *
 * This will return a pointer to the global void* representing
 * an ooc class. This can be used to test of two objects are of the same type,
 * or even to create new classes of the same type with new.
 * @param self ooc object
 * @return ooc class of a given ooc object
 */
const void* type(const void* self);

/**
 * @brief delete an ooc object
 *
 * This will call the object destructor if present, or simply
 * free the memory if no destructor is present
 * @param object
 */
void del(const void* self);

/**
 * @brief get size in bytes
 *
 * Get the full size in bytes of an ooc object.
 * This is implementation defined, and may not include the
 * size of items contained in a collection for example
 * @param self
 * @return size in bytes of object
 */
size_t size(const void* self);

/**
 * @brief get class name
 *
 * If supported, return the class name such as "String" or "HashMap"
 * as a c string. This is used mostly internally for error reporting.
 *
 * @param self ooc object instance
 * @return object name as c string
 */
const char* class_name(const void* self);

/**
 * @brief
 * Print a human friendly representation of an object to stdout
 * @param self
 */
void print(const void* self);


/**
 * @brief get the length of a collection object
 *
 * This function is usually implemented for objects that are collections of other objects.
 * For example, List, Tuple, HashMap, and File.
 * @param self ooc Collection object
 * @return number of items in collection
 */
size_t len(const void* self);

/**
 * @brief copy an object
 *
 * Perform a dep copy of an object. This is only supported by some types.
 *
 * @param self ooc object
 * @return copy of self
 */
const void* copy(const void* self);

/**
 * @brief compare two objects for equality
 *
 * Some objects have support for customized equality. In this case,
 * equality will be based on the contents of the objects. For example,
 * two different String objects with the same contents will compare as
 * equal.
 *
 * If objects don't implement a special copy, pointers will be compared.
 *
 * @param self
 * @param other
 * @return true if the objects compare as equal
 */
bool equals(const void* self, const void* other);

typedef enum {
    COMPARE_GT,
    COMPARE_EQ,
    COMPARE_LT
} CompareValue;

/**
 * @brief compare two objects for ordering/sorting
 *
 * This function will compare two objects for ordering. Not all objects
 * support ordering.
 *
 * For example, numbers have a natural order. Strings can be sorted in alphabetic
 * order, but it doesn't make sense to compare two Files.
 *
 * Objects supporting ordering can be sorted in collections that maintain ordering,
 * such as Lists.
 *
 * Note: This function will use the comparison function from the left argument.
 *       Usually, it only makes sense to compare objects of the same type, but for now,
 *       this is left somewhat underspecified, and the lhs comparison function needs
 *       to determine what to do.
 *
 *
 * @param lhs
 * @param rhs
 * @return COMPARE_GT if lhs > rhs
 *         COMPARE_EQ if lhs == rhs
 *         COMPARE_LT if lhs < rhs
 */
CompareValue compare(const void* lhs, const void* rhs);


typedef enum{
    SORT_ASCENDING,
    SORT_DESCENDING
} SortDirection;

void _obj_sort(size_t argc, ...);

/**
 * @brief Sort a collection in place
 * @param direction either SORT_ASCENDING, or SORT_DESCENDING
 * @param self
 */
#define obj_sort(...) _obj_sort(PP_NARG(__VA_ARGS__), __VA_ARGS__)

const void* sorted(const void* self);


/**
 * @brief return a c string of an object
 *
 * @param self ooc Object
 * @return c string
 */
const char* c_str(const void* self);


/**
 * @brief return an ooc String object representing self
 * @param self
 * @return ooc String object
 */
const void* to_String(const void* self);

/**
 * @brief add an item to the end of an ordered collection
 *
 * For a list, append a new item. For a string, concatenate a string.
 *
 * **Example**
 *
 *      const void *list = new(List);
 *      append(list, new(String, "One"));
 *      append(list, new(String, "Two"));
 *      print(list);
 *
 * @param self ooc Collection object
 * @param other object to append
 * @return self
 */
const void* append(const void* self, const void* other);

/**
 * @brief Indexed assignment into a collection
 *
 * set_item(collection, key, value) is equivalent to
 * collection[key] = value in python
 *
 * **Example**
 *
 *      const struct HashMap* map = new(HashMap);
 *      const struct String* key = new(String, "name");
 *      const struct String* name = new(string, "Isaac Newton");
 *
 *      set_item(map, key, name);
 *      assert(equals(name, get_item(map, key)));
 *
 * @param self ooc Collection
 * @param key
 * @param item
 * @return self
 */
void* set_item(const void* self,
               const void* key,
               const void* item);

/**
 * @brief indexed access into a collection
 *
 * value = get(collection, key)
 * value = collection[key]
 *
 * **Example**
 * @see get_item
 *
 * @note Right now, HashMaps return NULL if an item wasn't found.
 *       This might change in future versions, to perhaps raise an error.
 *
 * @param self ooc Collection
 * @param key ooc object key
 * @return item from collection or NULL if it wasn't present
 */
const void* get_item(const void* self,
                     const void* key);

/**
 * @brief create an Iterator object
 *
 * Iterators enable accessing items in a collection
 * one at a time.
 *
 * Use the returned iterator with @see next
 *
 * @param self ooc Collection
 * @return iterator for ooc Collection
 */
const void* iter(const void* self);

/**
 * @brief get an internal hash of an object
 *
 * The hash algorithm is different for different kinds of objects.
 *
 * Note: A hash may or may not be consistent between multiple
 *       runs of a program.
 *
 *       The returned hash can be used by user programs. It
 *       is used internally by the HashMap to determine if objects
 *       are unique. The HashMap will also use equals, to ensure different
 *       objects will not overwrite each other.
 *
 * **Example**
 *
 *      const size_t NUM_ITERATIONS = 1000;
 *      const struct String *strings[NUM_ITERATIONS];
 *
 *      const void *list = new(List);
 *
 *      for (size_t i = 0; i < NUM_ITERATIONS; i++) {
 *          char test_string[10];
 *          fill_random_str(test_string, sizeof(test_string));
 *          strings[i] = typed_new(String, test_string);
 *
 *          ck_assert_int_eq(i, len(list));
 *          append(list, strings[i]);
 *      }
 *
 *      const void *list_it = iter(list);
 *      for (size_t i = 0; i < NUM_ITERATIONS; i++) {
 *          const struct String *string = next(list_it);
 *          ck_assert(equals(string, strings[i]));
 *          del(strings[i]);
 *      }
 *      del(list);
 *
 * @see tests/check_utils.c for fill_random_str implementation
 *
 * @param self object to hash
 * @return integer hash used internally by HashMaps
 */
uint64_t hash(const void* self);

/**
 * @brief get the next item from an Iterator
 *
 * @param self ooc Iterator
 * @return  next item, or NULL
 */
const void* next(const void* self);

/**
 * @brief delete an item from a collection
 *
 * @param self
 * @param key
 */
void del_item(const void* self, const void* key);

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
void keys(const void* self);

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
void values(const void* self);

#define typed_new(class_name, ...) \
    (const struct class_name*) new(class_name, __VA_ARGS__)

#endif
