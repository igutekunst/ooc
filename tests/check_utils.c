//
// Created by Isaac Gutekunst on 9/26/19.
//

#include <ooc/object.h>
#include "check_utils.h"

void fill_random_chars(char* str, size_t len) {
   for (size_t i = 0; i < len; i++)  {
       char c = 0;
       do {
           c = 'a' + random() % 26;
       } while (c == 0);

      str[i] = c;
   }
}