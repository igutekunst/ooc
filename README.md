ooc
===

Experimenting with Object Oriented C

To install, do the usual dance.
````
./configure
make
make install
````

This will install a useless program named `main` It does nothing useful. Updates will be coming.


libOOC
====

To install just the library without the usefless executable

````
cd libooc
./configure
make
make install
````

You can now compile programs using libOOC by including <libooc/object.h> and <libooc/{the object you want}.h>

Compile with the -llibooc flag

Example 
````
#include <libooc/hashmap.h>

int main(int argc, char * argv[]) {
    struct HashMap * h = new(HashMap);
    /// etc
}

````

The readme is a work in progress. The project is not really intended for public consumption yet, more as a
notbook of my ideas...
