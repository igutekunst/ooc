#include "ooc/ooc.h"

int ooc_add(int a, int b) {
    return a + b;
}

const char* ooc_version(void) {
    static const char version[] = "ooc version 0.1.0";
    return version;
} 