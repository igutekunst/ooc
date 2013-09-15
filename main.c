#include <obj_point.h>
#include <obj_person.h>
#include <obj_string.h>
#include <obj_Int.h>
#include <obj_hashmap.h>
#include <assert.h>
int main(int argc, char * argv[]){
    const struct Point * a = new(Point, 5., 7.);
    const struct String *first   = new(String, "isaac");
    const struct String *cool   = new(String, "cool");
    
    const struct String * name = new(String, "Isaac Harrison Gutekunst");
    const struct String * tim = new(String, "Timothy R Mickel");
    const struct String * name_again  = copy(name);


    // Testing HashMaps
    
    const struct HashMap * h = new(HashMap);
    int i; 
    for(i = 0; i < 64 ; i++){
        char c[3];
        sprintf(c, "%d", i);
        const struct String * key = new(String, c);
        const struct  String * new_item = append(name, key);
        insert(h, key, new_item) ;
        //make sure same item is returned every time
        assert(new_item == get(h, key));
    }
    printf("seems good\n");
return 0;

}
