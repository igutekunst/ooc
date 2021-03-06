#include <assert.h>
#include <ooc/object.h>
#include <ooc/string.h>
#include "object_internal.h"

const struct ClassHeader Class = {
        .magic = MAGIC
};

/**
 *
 *
 * @param class OOC class to instantiate
 * @param ... arguments to initialize class
 * @return
 */
const void* _new(size_t argc, ...) {
    va_list args;
    va_start(args, argc);

    assert(argc != 0);

    const void* const _class = va_arg(args, const void*);

    const struct ClassHeader* class = (struct ClassHeader*) _class;
    if (!class || class->magic != MAGIC) {
        fprintf(stderr, "new called with invalid class\n");
        exit(1);
    }
    struct ObjectHeader* new_object = (struct ObjectHeader*) malloc(class->size);
    if (class->object_init) {
        //TODO remove extra initialization from specific class constructors
        new_object->class = class;
        new_object = (struct ObjectHeader*) class->object_init(new_object, argc - 1, args);
    } else {
        printf("No default constructor. Allocating %zu bytes\n", class->size);
    }
    va_end(args);
    return new_object;
}


void del(const void* _object) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_object, "Attempt to delete non object\n"))) {
        if (class->object_deinit) {
            class->object_deinit(_object);
        } else {
            free((void*) _object);
        }
    }
}

const char* c_str_repr(const void* _self) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_self, "str called on non object\n"))) {
        if (class->c_str_repr) {
            return class->c_str_repr(_self);
        } else
            return class->object_name;
    }
    return NULL;
}

const char* c_str(const void* _self) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_self, "str called on non object\n"))) {
        if (class->c_str) {
            return class->c_str(_self);
        } else
            return class->object_name;
    }
    return NULL;
}

const void* to_String(const void* _object) {
    return new(String, c_str(_object));

}


void print(const void* _self) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_self, "Attempted to print non object\n"))) {
        if (class->print) {
            class->print(_self);
        } else if (class->c_str) {
            printf("%s\n", class->c_str(_self));
        } else
            printf("Object at %p\n", _self);
    }
}


size_t size(const void* _self) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_self, "Attempted to get type of non object\n"))) {
        if (class->get_size)
            return class->get_size(_self);
        return class->size;
    }
    return 0;
}


const void* type(const void* _self) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_self, "Attempted to get type of non object\n")))
        return *(struct ClassHeader**) _self;
    return NULL;
}


size_t len(const void* _self) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_self, "Attempted to get len of non object\n"))) {
        if (class->get_len)
            return class->get_len(_self);
        else {
            fprintf(stderr, "TypeError: %s does not support len\n", class_name(_self));
            exit(1);
        }
    }
    return 0;
}


const char* class_name(const void* _self) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_self, "Attempted to get name of invalid object\n"))) {
        if (class->object_name)
            return class->object_name;
        else {
            printf("TypeError: object does not support name\n.");
            exit(1);
        }
    }
    return 0;
}


const void* copy(const void* _self) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_self, "Attempted to get len of non object\n"))) {
        if (class->copy)
            return class->copy(_self);
        else {
            printf("TypeError: object does not support copying\n");
            exit(1);
        }
    }
    return NULL;
}


bool equals(const void* _self, const void* _other) {
    const struct ClassHeader* class;
    const struct ClassHeader* other;

    if ((class = get_class_header_msg(_self, "Attempted to compare non object"))) {
        other = get_class_header_msg(_other, "attempted to compare non object\n");
        if (class->equals) {
            return class->equals(_self, _other);
        } else {
            return _self == _other;
        }
    }
    return NULL;
}

CompareValue compare(const void* _self, const void* _other) {
    const struct ClassHeader* class;
    if ((class = get_class_header_msg(_self, "Attempted to compare non object"))) {
        if (class->compare == NULL) {
            fprintf(stderr, "%s does not support comparison\n", class_name(_self));
            exit(EXIT_FAILURE);
        }
        return class->compare(_self, _other);
    }

    // Should not get here
    assert(false);
}

/**
 * @brief Sort a collection in place
 * @param _self
 */
void _obj_sort(size_t argc, ...) {
    va_list args;
    va_start(args, argc);
    assert(argc == 1 || argc == 2);

    const void* const _self = va_arg(args, const void*);

    if (get_class_header_msg(_self, "Failed to create iterator from non collection\n")) {
        const struct ClassHeader* self = get_class_header(_self);
        if (self->sort == NULL) {
            fprintf(stderr, "Class %s does not support sort.\n", class_name(_self));
            exit(EXIT_FAILURE);

        }
        SortDirection sort_direction = SORT_ASCENDING;
        if (argc == 2) {
            sort_direction = (SortDirection) va_arg(args, SortDirection);
        }
        self->sort(_self, sort_direction);
    }
    va_end(args);
}


const void* append(const void* _self, const void* _other) {
    //TODO string concatenation should probably be a different function
    const struct ClassHeader* class;

    if ((class = get_class_header_msg(_self, "Attempted to append non object\n"))) {
        if (class->append) {
            return class->append(_self, _other);
        } else {
            fprintf(stderr, "Type %s does not support append\n", class_name(_self));
            exit(EXIT_FAILURE);
        }
    }

    // Should not get here
    assert(false);
}


inline struct ClassHeader* get_class_header_msg(const void* _self, const char* message) {
    if (_self == NULL) {
        fprintf(stderr, "get_class_header_msg called with NULL object. Message: %s\n", message);
        exit(1);
    }
    struct ClassHeader* class = *(struct ClassHeader**) _self;
    if (class && class->magic == MAGIC) {
        return class;
    }
    if (message) {
        fprintf(stderr, "%s", message);
        exit(1);
    } else {
        fprintf(stderr, "get_class_header_msg failed for unknown reason");
        exit(1);
    }
}


inline const struct ClassHeader* get_obj_type(const void* _self, const void* class, const char* message) {
    struct ClassHeader* class_header = *(struct ClassHeader**) _self;
    if (class_header && class_header->magic == MAGIC) {
        if (class_header == class) {
            return class;
        }
    }
    if (message) {
        fprintf(stderr, "%s", message);
        exit(1);
    } else {
        fprintf(stderr, "get_class_header_msg failed for unknown reason");
        exit(1);
    }

}


inline const struct ClassHeader* get_class_header(const void* _self) {
    return *(struct ClassHeader**) _self;
}

uint64_t hash(const void* _self) {

    if (get_class_header_msg(_self, "hash called with invalid object")) {
        const struct ClassHeader* self = get_class_header(_self);
        return self->hash(_self);
    }
    return 0;
}

void* set_item(const void* _self,
               const void* _key,
               const void* _other) {

    if (get_class_header_msg(_self, "Failed to insert into non collection\n")) {
        const struct ClassHeader* self = get_class_header(_self);
        assert(self->insert);
        self->insert(_self, _key, _other);
    }
    return NULL;
}

void del_item(const void* _self, const void* _key) {
    if (get_class_header_msg(_self, "Failed to get from non collection\n")) {
        const struct ClassHeader* self = get_class_header(_self);
        if (self->del_item) {
            self->del_item(_self, _key);
        } else {
            printf("TypeError: object does not support del. \n");
            exit(1);
        }
    }
}

// TODO consider a shortcut for get_item(object, new(Int, xx)) for handling integer keys for convenience
const void* get_item(const void* _self, const void* _key) {
    if (get_class_header_msg(_self, "get_item called on invalid object")) {
        const struct ClassHeader* self = get_class_header(_self);
        if (self->get_item == NULL) {
            fprintf(stderr, "%s does not support get_item\n", class_name(_self));
            exit(EXIT_FAILURE);
        }
        return self->get_item(_self, _key);
    }
    return NULL;
}

const void* iter(const void* _self) {
    if (get_class_header_msg(_self, "Failed to create iterator from non collection\n")) {
        const struct ClassHeader* self = get_class_header(_self);
        if (self->iter == NULL) {
            fprintf(stderr, "Class %s does not support iteration.\n", class_name(_self));
            exit(EXIT_FAILURE);

        }
        return self->iter(_self);
    }
    return NULL;
}

const void* next(const void* _self) {
    if (get_class_header_msg(_self, "Failed to get next on non object\n")) {
        const struct ClassHeader* self = get_class_header(_self);
        if (self->next) {
            return self->next(_self);
        }
    }
    return NULL;
}

