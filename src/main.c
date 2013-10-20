#include "point.h"
#include "person.h"
#include "string.h"
#include "Int.h"
#include "hashmap.h"
#include <assert.h>
#include <config.h>
int usage(){
    printf("This is " PACKAGE_STRING ".");
    return 0;
}
int main(int argc, char * argv[]){
    const struct Point * a = new(Point, 5., 7.);
    const struct String *first   = new(String, "isaac");
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
    exit(0);
    printf("Made iterator \n");



    
    const struct HashMap * h = new(HashMap);
    int i; 
    for(i = 0; i < 64 ; i++){
        char c[10];
        sprintf(c, "key%d", i);
        const struct String * key = new(String, c);
        const struct  String * new_item = append(name, key);
        insert(h, key, new_item) ;
        //make sure same item is returned every time
        assert(new_item == get_item(h, key));
    }
    printf("seems good\n");
return 0;

}
