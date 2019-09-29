//
// Created by Isaac Gutekunst on 2019-09-29.
//

#ifndef OOC_TRAIT_STRING_H
#define OOC_TRAIT_STRING_H

#include <stdlib.h>

struct OOCTraitString {
    const void* (* string_split)(const void* string, const void* _sep);


    //TODO maybe this goes somewhere else, like in collections
    //TODO maybe use Int instead
    const void* (* string_slice)(const void* string, ssize_t start, ssize_t end);
};

#endif //OOC_TRAIT_STRING_H
