#include <point.h>
#include <person.h>
int main(int argc, char * argv[]){
    struct Point * a = new(Point, 5., 7.);
    struct Person * rob = new(Person, "Robert", "Johnson", 21);
    print(a);
    print(rob);
    delete(a);
    delete(rob);

    return 0;

}
