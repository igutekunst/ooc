#include <obj_point.h>
#include <obj_person.h>
#include <obj_string.h>
#include <obj_Int.h>
int main(int argc, char * argv[]){
    const struct Point * a = new(Point, 5., 7.);
    const struct String *name;
    const struct String *first   = new(String, "Isaac ");
    const struct String *last   = new(String, "Gutekunst\n");
    
    name = append(first, last);
    print (name);
    const struct String * name_again  = copy(name);

    return 0;

}
