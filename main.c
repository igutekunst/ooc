#include <obj_point.h>
#include <obj_person.h>
#include <obj_string.h>
#include <obj_Int.h>
#include <obj_hashmap.h>
int main(int argc, char * argv[]){
    const struct Point * a = new(Point, 5., 7.);
    const struct String *first   = new(String, "Isaac ");
    const struct String *last   = new(String, "Gutekunst\n");
    
    const struct String * name = append(first, last);
    print (name);
    const struct String * name_again  = copy(name);


    // Testing HashMaps
    
    const struct HashMap * h = new(HashMap);
    
    insert(h, first, name);
    //struct String * out = get(h, first);

    return 0;

}
