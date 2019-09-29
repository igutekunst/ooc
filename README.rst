.. _mainpage:

Objected Oriented C
===================


.. image:: https://img.shields.io/badge/License-GPLv3-blue.svg

`Documentation Table of Contents <http://igutekunst.github.io/OOC/intro.html>`

Experimenting with `Object Oriented C`.

This library is a toy implementation of “objects” in C, with the idea to
enable dynamic dispatch and polymorphism of a set of carefully written `classes`.

This library aims to enable:

- Less boilerplate code to do common tasks
- A friendlier standard library
- A `batteries included` C environment with dynamic typing

The design of both the API and internals are inspired by Python.
No attempt has been made to mimic python APIs exactly.
Future versions may integrate with Python.

This library is experimental and has not been used in any
real projects as far as I know.

In the future, I hope to use OOC to build some larger personal projects and improve it to the
point where other people will chose to use it.


Working Modules
---------------

-  HashMap
-  String
-  Int
-  List
-  Tuple
-  File


Getting Started Tutorial
------------------------

See ``examples/count_words.c``



This test shows a more complete usage of the OOC library to solve a common
interview question.

**Problem Statement:**
Given a file full of words, count the time every word occurs. For bonus
points, print the counts in sorted order.

A word is defined as one or more non-space characters. For now, only consider the space character
as a separator and ignore the tab character.

For Example, given the following file::

    one two three four five
    two three four five
    three four five
    four five
    five

The output should be::

     5, five
     4, four
     3, three
     2, two
     1, one


Running Example
+++++++++++++++
If you only want to run the example, run the following commands::

    mkdir cmake-build
    cd cmake-build
    make
    ./count_words test_files/test-3.txt

Output::

     5, five
     4, four
     3, three
     2, two
     1, one

Another Example::

    ./count_words test_files/declaration.txt

Output::

     80, of
     76, the
     64, to
     56, and
     ... etc

Step By Step
++++++++++++

First we will include several headers that we will need later. ::

   #include <OOC/list.h>
   #include <OOC/hashmap.h>
   #include <OOC/math.h>
   #include <OOC/int.h>
   #include <OOC/string.h>
   #include <OOC/object.h>
   #include <OOC/tuple.h>
   #include <OOC/file.h>

Then we create a basic C program main function. Our function will take one argument,
a filename to process. ::

   int main(int argc, char* argv[]) {
       if (argc != 2) {
           printf("Usage: %s file\n", argv[1]);
           exit(EXIT_FAILURE);
       }

        return (EXIT_SUCCESS);
   }

The general idea is to store the count of all seen words in a HashMap.
Next we sort the results for printing.

First we open a file, and read one line at a time ::

       while (full_line != NULL) {

           // Remove trailing newline
           const struct String* line = string_slice(full_line, 0, -1);
           del(full_line);

           full_line = file_read_line(f);
       }


Then we split the line into words using the space character,
and then iterate over the words. ::

       while (full_line != NULL) {

           // Remove trailing newline
           const struct String* line = string_slice(full_line, 0, -1);
           del(full_line);

           const struct List* list = string_split(line);

           // Iterate over the list of words, inserting them into the HashMap
           const void* it = iter(list);
           const struct String* string;
           for (string = next(it); string != NULL; string = next(it)) {
                // Process words in here
           }
       }

If a word has not been seen before, a 0 is inserted into the HashMap using
the word as a key. Then the count is incremented. ::

    while (full_line != NULL) {

        // Remove trailing newline
        const struct String* line = string_slice(full_line, 0, -1);
        del(full_line);

        const struct List* list = string_split(line);

        // Iterate over the list of words, inserting them into the HashMap
        const void* it = iter(list);
        const struct String* string;
        for (string = next(it); string != NULL; string = next(it)) {
            const struct Int* temp_count = NULL;
            if ((temp_count = get_item(counts, string)) == NULL) {
                temp_count = new(Int, 0);
                set_item(counts, string, temp_count);
            }
            set_item(counts, string, obj_add(temp_count, new(Int, 1)));
            del(temp_count);
        }
        del(list);

Note that we are leaking a bit of memory by using temporary Int objects.

Next we put the results into a List of Tuple objects for sorting. ::

    // Put the results into a List of Tuples for sorting
    const void* count_it = iter(counts);

    const struct List* counts_list = new(List);

    for (const void* key = next(count_it); key != NULL; key = next(count_it)) {
        const struct Tuple* t = new(Tuple, get_item(counts, key), key);
        append(counts_list, t);
    }
    obj_sort(counts_list, SORT_DESCENDING);

At the end, the totals are printed by looping over the HashMap keys.
The keys are sorted by getting a tuple for each (key,value) pair, and sorting the list ::

       // Iterate over the tuples for printing
       const void* ci = iter(counts_list);
       const struct Tuple* t;

       for (t = next(ci); t != NULL; t = next(ci)) {
           printf(" %d, %-14s\n", obj_to_int(get_item(t, new(Int, 0))), c_str(get_item(t, new(Int, 1))));
       }
       printf("\n");

Note: The OOC library does not support referencing counting, and as such, allocates memory that we don't free.
Notable examples are temporary variables used to make calculations.
This is because almost all OOC functions require OOC objects as arguments.

In the future, referencing counting will make it easier to avoid memory leaks.

Full Source Code
++++++++++++++++
::

       const struct HashMap* counts = new(HashMap);
       const struct File* f = new(File, argv[1]);

       const struct String* full_line = file_read_line(f);


       while (full_line != NULL) {

           // Remove trailing newline
           const struct String* line = string_slice(full_line, 0, -1);
           del(full_line);

           const struct List* list = string_split(line);

           // Iterate over the list of words, inserting them into the HashMap
           const void* it = iter(list);
           const struct String* string;
           for (string = next(it); string != NULL; string = next(it)) {
               const struct Int* temp_count = NULL;
               if ((temp_count = get_item(counts, string)) == NULL) {
                   temp_count = new(Int, 0);
                   set_item(counts, string, temp_count);
               }
               set_item(counts, string, obj_add(temp_count, new(Int, 1)));
               del(temp_count);
           }
           del(list);
           full_line = file_read_line(f);

       }


       // Put the results into a List of Tuples for sorting
       const void* count_it = iter(counts);

       const struct List* counts_list = new(List);

       for (const void* key = next(count_it); key != NULL; key = next(count_it)) {
           const struct Tuple* t = new(Tuple, get_item(counts, key), key);
           append(counts_list, t);
       }
       obj_sort(counts_list, SORT_DESCENDING);


       // Iterate over the tuples for printing
       const void* ci = iter(counts_list);
       const struct Tuple* t;

       for (t = next(ci); t != NULL; t = next(ci)) {
           printf(" %d, %-14s\n", obj_to_int(get_item(t, new(Int, 0))), c_str(get_item(t, new(Int, 1))));
       }
       printf("\n");


       // Note: There are several memory leaks. These are "intentional", because the
       // goal of the OOC project is first to develop okay ergonomics. After that, referencing counting
       // will be added, making it far easier to manage memory.
       del(count_it);
       del(ci);
       del(counts_list);
       del(counts);
       del(f);

       return EXIT_SUCCESS;
   }









