Technical Details
=================


All actions performed with OOC objects is done through a uniform API, exposed through a
set of header files. Core functionality is included in ``OOC/object.h``, with additional
functionality exposed through additional headers.

From a user's perspective, every OOC object is a const void*, or an opaque struct. This enables perfect
data hiding, allowing the API implementation to change without affecting user code.

Of course, this has the downside of breaking the C type system, which makes catching errors at compile time harder.

However, this enables the same set of functions to operate on a variety of types.

Implementation Details
----------------------

Every OOC ``Class`` has an associated :ref:`ClassHeader <class_header>` that describes its functionality.


Here's an abbreviated code sample

::

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

Here's the :ref:`ClassHeader <class_header>` for the ``String`` class

::

    struct String {
        struct ClassHeader* class;
        size_t size;
        size_t len;
        char* string_data;
    };

The interesting thing to note here is the first item in the struct is a ``ClassHeader*.``. This is a
crucial technique that allows a void* to be identified. After it is extracted, the correct methods can be called.

Every top level OOC function will perform a NULL check, and then cast the object into a ``ClassHeader*``.
This may involve some argument pre-processing, followed by a call to the
appropriate method stored as a function pointer in the :ref:`ClassHeader <class_header>`.

This function pointer will be implemented as a function in a per-class implementation file. For .. code-block:: c, `src/string.c` contains
the implementation of all the ``String`` functions.

Implementing an Instance
++++++++++++++++++++++++

The implementation of a given class is relatively straightforward, although it involves a good amount of boilerplate code,
which seems unavoidable in C.

Let's look at a simplified part of the ``String`` implementation

::

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
    2. The size of the object before initialization is specified. This is used by new to allocate memory.
    3. Some function pointers are in ``.class`` These are considered common, and implemented for many objects.
       Some more specific functionality is included in *Traits*, which are extensions to the ClassHeader, and will be
       discussed later.

Here's the full implementation of ``String__init``

::

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

The signature might look a bit confusing. There is the void* _self, which might make sense, but
there's also an ``argc``, and a ``va_list``. This is due to how ``new`` is implemented.

``new`` uses a bit of macro magic ( ``src/object_internal.h``) to capture the number of arguments passed to it, and
passes this in argc. Similarly, all the arguments are passed in va_arg.


Traits
++++++

Traits are a concept borrowed from languages such as Rust that have strong type systems.
Various types can implement different Traits, making them compatible with functions that require those traits.

For .. code-block:: c, a class could implement the ``Orderable``, trait, and immediately be sortable. Similarly, a class
could implement the ``Iterable`` trait, and be usable with functional programming functions such as ``map`` and ``reduce``.

Of course you lose a bunch of the value of *Traits* without having static typing, but it will enable better error messages
and runtime type checking.

Right now this isn't implemented, but it's easy to imagine some functions that check that arguments implement certain traits.

For .. code-block:: c::

    const void* String_append(const void* _self, const void* _other) {
        ASSERT_SUPPORTS_TRAIT(_other, ToSring);
        ...
    }

Or for a HashMap

::

    void internal_insert_HashMap(struct HashMap *self,
                             const void *_key,
                             const void *_value) {
        ASSERT_SUPPORTS_TRAIT(_other, Hashable, Equality);
    }

This would greatly simplify how type checking is done. This can be accomplished pretty easily by
adding some boolean fields to the ClassHeader, one for each `Trait` available in OOC. The assertion
could then easily verify the type supports some functionality.

Right now, OOC only performs type checking for the first argument for a function. It's done in the
top level function, for .. code-block:: c ``obj_insert(const void* _self, ...)``. This function ensures the
object _self  has a non-NULL unction pointer ``obj_insert``, but doesn't do any other checking.















