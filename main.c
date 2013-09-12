#include <obj_point.h>
#include <obj_person.h>
#include <obj_string.h>
int main(int argc, char * argv[]){
    struct Point * a = new(Point, 5., 7.);
    struct String *first   = new(String, "Isaac ");
    struct String *last   = new(String, "Gutekunst\n");
    first  = append(first, last);
    print(first);
    return 0;

}
