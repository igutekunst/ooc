.. _mainpage:

Objected Oriented C
===================

Experimenting with “Object Oriented C”.

This library is a toy implementation of “objects” in C, with the idea to
enable dynamic dispatch and polymorphism of a set of carefully written
“classes” .

This library aims to enable: \* Less boilerplate code to do common tasks
\* A friendlier standard library \* A “batteries included” C like
environment.

The design of both the API and internals are inspired by Python, though
no attempt has been made to mimic python APIs exactly, or interface with
Python, though this may be a goal in the future.

This library is completely experimental, and has not been used in any
real projects as far as I know.

It’s here to share some ideas with friends and the world. My hope is it
will be useful one day, at which point I will promote it with more
confidence!

Working Modules
---------------

-  HashMap
-  String
-  Int
-  List
-  Tuple

Example \```\` #include <ooc/list.h> #include <ooc/hashmap.h> #include
<ooc/math.h> #include <ooc/int.h> #include <ooc/string.h> #include
<ooc/object.h> #include <ooc/tuple.h> #include <ooc/file.h>

/*\* This test shows a more somewhat complete usage of the OOC library
to solve a common \* interview question. Problem Statement: Given a file
full of words, count the time every word occurs. For bonus \* points,
print the counts in sorted order. A word is defined as one or more non
space characters. For now, only the space character needs \* to be
considered, but also treating tab a separator is even better. @param \_i
*/ int main(int argc, char* argv[]) {

::

   if (argc != 2) {
       printf("Usage: %s file\n", argv[1]);
       exit(EXIT_FAILURE);
   }

   /*
    * The general idea here is to store the count of all seen words in a HashMap, and
    * then sort the results later for printing
    * First we open a file, and read on line at a time
    *
    * Then we split the line into words using the space character,
    * and then iterates over the words.
    *
    * If a word has not been seen before, a 0 is inserted into the HashMap using
    * the word as a key. Then the count is incremented.
    *
    * At the end, the totals are printed by looping over the HashTable keys.
    * The keys are sorted by getting a tuple for each (key,value) pair, and sorting the list
    *
    * Note: The ooc library does not support referencing counting, and as such, allocates
    * lots of memory that we don't free. Notable examples are temporary variables used
    * to make calculations, and because almost all ooc functions require ooc objects as arguments.
    *
    * In the future, referencing counting will make avoiding memory leaks much easier.
    */

   const struct HashMap* counts = new(HashMap);
   const struct File* f = new(File, argv[1]);

   const struct String* full_line = file_read_line(f);


   while (full_line != NULL) {

       // Remove trailing newline
       const struct String* line 