//
// Created by Isaac Gutekunst on 2019-09-28.
//

#ifndef OOC_TRAIT_FILE_H
#define OOC_TRAIT_FILE_H

#include <stdbool.h>
#include <stddef.h>

struct OOCTraitFile {
    const void* (* _file_read)(const void* file, size_t bytes_to_read, bool read_all);


    const void* (* file_read_line)(const void* _self);
};

#endif //OOC_TRAIT_FILE_H
