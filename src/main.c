#include <ooc/point.h>
#include <ooc/person.h>
#include <ooc/string.h>
#include <ooc/Int.h>
#include <ooc/hashmap.h>
#include <assert.h>

int main(int argc, char * argv[]){
    const struct Point * a = new(Point, 5., 7.);
    const struct String *first   = new(String, "Isaac");
    const struct String *cool   = new(String, "cool");
    
    const struct String * name = new(String, "Isaac Harrison Gutekunst");
    const struct String * tim = new(String, "Timothy R Mickel");
    const struct String * name_again  = copy(name);


    // Testing HashMaps
    const struct HashMap * h_iter_test  = new(HashMap);
    const struct String * s1 = new(String, "Value 1\n")  ;
    const struct String * s2 = new(String, "Value 2\n")  ;
    const struct String * s3 = new(String, "Value 3\n")  ;

    const struct String * k1 = new(String, "key1")  ;
    const struct String * k2 = new(String, "key2")  ;
    const struct String * k3 = new(String, "key3")  ;

    insert(h_iter_test, k1, s1);
    insert(h_iter_test, k2, s2);
    insert(h_iter_test, k3, s3);

    struct HashMap_iter * it =   iter(h_iter_test);

    for (void * item = next(it); item != NULL; item = next(it)){
        if (item){
            print(item);
        }
    }


    return 0;
}
