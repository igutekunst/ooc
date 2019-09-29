//
// Created by Isaac Gutekunst on 9/26/19.
//

#include <ooc/object.h>
#include "check_utils.h"

void fill_random_str(char* str, size_t len) {
   for (size_t i = 0; i < len -1; i++)  {
       char c = 0;
       do {
           c = (char) 'a' + random() % 25;
       } while (c == 0);

      str[i] = c;
   }
   str[len -1] = '\0';
}