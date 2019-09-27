#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "trait_math_internal.h"

#define MAGIC ((unsigned long ) 0xBD71472)

/**
 * All ooc classes have a class_header.
 *
 * The header provides function pointers to implementations of
 * all supported methods for a given class.
 *
 * Each object (instantiation of a class) has a pointer to the per-class
 * class_header.
 *
 * A minimum class must at least provide implementations for
 * magic, size, object_init, object_deinit and object_name
 *
 */
struct class_header {
    unsigned long magic;
    size_t size;

    /**
     * Get size of object in bytes
     * @param _self ooc object
     * @return  size in bytes
     */
    size_t (*get_size)(const void *_self);

    /**
     * Get number of items in container object
     * @param _self ooc object
     * @return number of items in container
     */
    size_t (*get_len)(const void *_self);

    /**
     * Create and initialize an object
     * Technically, _self needs to be the full class header for
     * a concrete type, which starts with a class_header, followed by
     * class specific fields.
     *
     * @param _self ooc class like String or HashMap.
     * @param args arguments to initialize object. Varies based on class type
     * @return
     */
    const void *(*object_init )(
            const void *_self,
            va_list args
    );

    /**
     * De-initialize and free resources used by _self.
     *
     * @param _self ooc object
     */
    void (*object_deinit)(const void *_self);

    /**
     * Print and object to stdout
     * @param _self ooc object
     */
    void (*print)(const void *_self);

    /**
     * Return the c string (char*) representation of
     * an object
     *
     * May be used by to_String to make an ooc String representation.
     * Usually it's not necessary to implement to_String separately.
     *
     * @param _self ooc object
     * @return c string representation
     */

    const char *(*str)(const void *_self);

    /**
     * Return String representation of object.
     *
     * May allocate a new String object, and may or
     * may not return a different String object each time.
     *
     * Usually it is sufficient to implement str.
     *
     * @param _self  ooc objet
     * @return ooc String object
     */
    void *(*to_String)(const void *_self);

    /**
     * Implement to enable appending items to an class.
     * If an class supports appending, it should probably
     * support either get, or iteration.
     *
     * @param _self ooc container object
     * @param _other ooc object supported by container
     */
    const void *(*append)(
            const void *_self,
            const void *_other
    );

    /**
     * Implement to allow objects to be compared with a custom function.
     * @param _self
     * @param _other
     * @return true if objects are equal. If not implemented, equals will compare pointers
     */
    bool (*equals)(
            const void *_self,
            const void *_other
    );

    /**
     * Return an iterator to support iteration.
     * @param _object ooc object
     * @return ooc iterator
     */
    const void *(*iter)(const void *_object);

    /**
     * Copy an ooc object.
     *
     * Implement functionality to copy an object.
     * The expectation is that it is deep copy.
     * Perhaps there should be a convention for shallow copies
     *
     * @param _self
     * @return copy of _self
     */
    const void *(*copy)(const void *_self);

    /**
     * Implement to enable indexed assignment.
     * insert(collection, key, value) is equivalent to
     * collection[key] = value
     *
     * @param _self ooc object
     * @param _key ooc object key to index into collection
     * @param item value to store at given index
     */
    void (*insert)(
            const void *_self,
            const void *_key,
            const void *item
    );

    /**
     * Implement to enable indexed access.
     * value = get(collection, key)
     * value = collection[key]
     *
     * Typically, key must be in collection, or this function
     * should exit early with an error.
     * @param _self ooc object
     * @param _key ooc object key
     * @return
     */
    const void *(*get)(
            const void *_self,
            const void *_key
    );

    /**
     * Implement to enabled indexed deletion from a collection.
     * @param _self ooc object
     * @param key key to delete
     */
    void (*del_item)(
            const void *_self,
            const void *key
    );


    /**
     * Implement to enable storing in a hash table.
     *
     * Note: as of now, HashTables will also use equals() to
     * confirm keys are unique. If hash is not implemented,
     * as of now, the HashTable does not support it. This may change
     * in the future.
     *
     * @param _self ooc object
     * @return 64 bit user defined hash
     */
    uint64_t (*hash)(const void *_self);

    /**
     * Implement to support iteration.
     * You must implement iter() and
     * next() to support iteration.
     *
     * @param _self iterable returned from iter()
     * @return next item in iteration.
     */
    const void *(*next)(const void *_self);

    /**
     * Object name as C string
     */
    const char *object_name;

    /**
     * Traits
     */
     struct OOCTraitMath math;

};

