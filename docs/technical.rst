Technical Details
=================


All actions performed with OOC objects is done through a uniform API, exposed through a
set of header files. Core functionality is included in ``ooc/object.h``, with additional
functionality exposed through additional headers.

From a user's perspective, every OOC object is a const void*, or an opaque struct. This enables perfect
data hiding, allowing the API implementation to change without affecting user code.

Of course, this has the downside of `breaking` the C type system, which makes catching errors at compile time harder.

What this enables however, is using the same set of functions to operate on a variety of types.

Implementation Details
----------------------

Every OOC ``Class`` has an associated :ref:`ClassHeader <class_header>` that describes its functionality.


Here's an abbreviated code sample::

    struct ClassHeader {
        unsigned long magic;
        size_t size;
        const char *object_name;
        size_t (*get_size)(const void *_self);
        size_t (*get_len)(const void *_self);
    }

The magic number is used to determine if a ``void*`` passed to a function is really an OOC object, or some
other non-NULL pointer.

Instances
+++++++++

Every instance of an OOC ``Class``, called objects, has a pointer to its associated header, which
will do dynamic dispatch to the correct "method" at runtime.

Here's the :ref:`ClassHeader <class_header>` for the ``String`` class:::

    struct String {
        struct ClassHeader* class;
        size_t size;
        size_t len;
        char* string_data;
    };

The interesting thing to note here is the first item in the struct is a ``ClassHeader*.``. This is the
crucial technique that allows a a given object, in the form of a void* to identified, and the correct methods
to be called.

Every top level OOC function will first do a NULL check, and then cast the object into a ``ClassHeader*``, to
determine what to do next. This might involve some pre-processing of arguments, followed by a call to the
appropriate method, stored as a function pointer in the :ref:`ClassHeader <class_header>`.

This function pointer will be implemented as a function in per-class implementation file. For example, `src/string.c` contains
the implementation of all the ``String`` functions.

Implementing an Instance
++++++++++++++++++++++++

The implementation of a given class is relatively straightforward, though involves a good amount of boilerplate code,
which seems unavoidable in C.

Let's look at a simplified part of the ``String`` implementation::

    struct StringClass string_class = {
            .class = {
                    .magic = MAGIC,
                    .size = sizeof(struct String),
                    .object_init = String_init,
                    .get_size = String_get_size,
                    .equals = String_equal,
                    .object_name = "String",
                    .string = {
                            .string_split = String_split,
                            .string_slice = String_slice
                    }
            }

    };

Here we can see a few important things:
    1. The MAGIC must be set here. Otherwise, ``new`` may try to construct objects out of invalid pointers.
    2. The size of the object before initialization is specified. This is used by new to allocate memoery
    3. Some function pointers are in ``.class``, and are considered common, and implemented for many objects.
       Some more specific functionality is included in *Traits*, which are extensions to the ClassHeader, and will be
       discussed later.

Here's the full implementation of ``String__init``::

    const void* String_init(const void* _self, size_t argc, va_list args) {
        (void) argc;
        struct String* self = (struct String*) _self;
        const char* data = va_arg(args, const char *);
        if (!data) {
            free(self);
            return NULL;
        }

        // TODO allow string to be created using data without copying

        size_t len = strlen(data);
        size_t size = sizeof(struct String) + len + 1;
        self = (struct String*) realloc(self, size);
        self->class = String;
        void* space = (void*) &self[1];
        self->string_data = strcpy(space, data);
        self->size = size;
        self->len = len;
        return self;
    }

Here we can again see a few important things.

The signature might look a bit weird. There is the void* _self, which might make sense, but
there's also an ``argc``, and a ``va_list``. This is do to how ``new`` is implemented.

``new`` uses a bit of macro magic ( ``src/object_internal.h``) to capture the number of arguments passed to it, and
passes this in argc. Similarly, all the arguments are passed in va_arg.


Traits
++++++

Traits are a concept borrowed from languages such as Rust that have strong type systems. The idea, is that
various types can implement different Traits, and then be compatible with functions that require those traits.

For example, a class could implement the ``Orderable``, trait, and immediately be sortable. Similarly, a class
could implement the ``Iterable`` trait, and be usable with functional programming functions such as ``map`` and ``reduce``.

Of course you lose a bunch of the value of *Traits* without having static typing, but it will enable better error messages
and runtime type checking.

Right now this isn't implemented, but it's easy to imagine some functions that check that arguments implement certain traits.

For example::

    const void* String_append(const void* _self, const void* _other) {
        ASSERT_SUPPORTS_TRAIT(_other, ToSring);
        ...
    }

Or for a HashMap::

    void internal_insert_HashMap(struct HashMap *self,
                             const void *_key,
                             const void *_value) {
        ASSERT_SUPPORTS_TRAIT(_other, Hashable, Equality);
    }

This would greatly simply how type checking is done. This can be accomplished pretty easily by
adding some boolean fields to the ClassHeader, one for each `Trait` available in OOC. The assertion
would then easily be able to verify the type supports some functionality.

Right now, most type checking is done only for the first argument for a function. It's done in the
top level function, for example ``obj_insert(const void* _self, ...)``. This function ensures the
object _self at least has a non-NULL unction pointer ``obj_insert``.















