#include "point.h"
#include "person.h"
#include "string.h"
int main(int argc, char * argv[]){
    struct Point * a = new(Point, 5., 7.);
    void * name = new(String, "YOLO");
    printf("str(name): %s bitches\n", str(a));



    return 0;

}
