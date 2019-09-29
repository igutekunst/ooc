OOC API Documentation
=====================


Basic Object Management
-----------------------

OOC objects are created with `new`. This is the most commonly used function when
using OOC. New was named after new in C++, and performs a similar allocation and initialization.


Most objects also support a handful of basic functions to print t


.. doxygendefine:: new
	:project: ooc

.. doxygenfunction:: del
	:project: ooc

.. doxygenfunction:: type
	:project: ooc

.. doxygenfunction:: get_obj_type
	:project: ooc

.. doxygenfunction:: size
	:project: ooc

.. doxygenfunction:: class_name
	:project: ooc

.. doxygenfunction:: print
	:project: ooc

.. doxygenfunction:: copy
	:project: ooc

.. doxygenfunction:: equals
	:project: ooc

.. doxygenenum:: CompareValue
	:project: ooc

.. doxygenfunction:: compare
	:project: ooc


Collections
-----------

OOC has several functions and built in types that behave like *collections*. Collections are
objects that support a subset of features such as len, iter, next, get_item, del_item.

These functions, if available will work on any collection, but will behave differently. For example,
append(string, new(String, "world")), is different than append(list, new(String, "world")). In the
string example, a new String is return, with string concatenated with "world". In the list example,
the list will have a new entry, the String with contents "world".

.. doxygenfunction:: len
	:project: ooc

.. doxygenfunction:: get_item
	:project: ooc

.. doxygenfunction:: set_item
	:project: ooc

.. doxygenfunction:: append
	:project: ooc

.. doxygendefine:: obj_sort
	:project: ooc

Iteration
+++++++++

OOC has basic support for iteration. Iteration allows us to traverse a collection
one item at a time, without needing to know indexes. In the future it will allow
functional features, such as ``map`` and ``reduce``.

.. doxygenfunction:: iter
	:project: ooc

.. doxygenfunction:: next
	:project: ooc


Hashing
+++++++

Some objects support hashing. Objects that support hashing
can be stored in HashMaps.

.. doxygenfunction:: hash
	:project: ooc

Internal Functions
++++++++++++++++++

.. doxygenfunction:: get_class_header_msg
	:project: ooc

