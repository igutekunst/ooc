#include <point.h>
#include <person.h>
int main(int argc, char * argv[]){
    struct Point * a = new(Point, 5., 7.);
    //printf("%lf %lf\n", a->x, a->y);
    print(a);
    //delete(a);

    return 0;

}
