Objected Oriented C {#mainpage}
===================

Experimenting with "Object Oriented C".

This library is a toy implementation of "objects" in C, with the idea
to enable dynamic dispatch and polymorphism of a set of carefully written "classes" .

This library aims to enable:
* Less boilerplate code to do common tasks
* A friendlier standard library
* A "batteries included" C like environment.


The design of both the API and internals are inspired by Python, though no attempt has been
made to mimic python APIs exactly, or interface with Python, though this may be a goal in the 
future. 

This library is completely experimental, and has not been used in any real projects as far as I know. 

It's here to share some ideas with friends and the world. My hope is it will be useful one day, at which point
I will promote it with more confidence!

Working Modules
---------------
* HashMap
* String
* Int

Example 
````
#include <libooc/hashmap.h>

int main(int argc, char * argv[]) {
    struct HashMap * h = new(HashMap);
    /// etc
}

````

Future Work
-----------
* Add functionality to Int
* Add Float and BigInt support
* Add referencing counting
* Traits
* Add functional functions such as map, reduce
* Add internal argument type checking macros/functions
* Add error handling system, and unify error messages
* Add global free list, so you can call `del_all` or similar
* Add more functions to String

In Progress
--------------
* Add list
* Flesh out list
* Add File type that supports iteration
* Adding sorting to list (seems to work)
* Add Tuple type
