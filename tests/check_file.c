//
// Created by Isaac Gutekunst on 2019-09-28.
//

#include "check_file.h"
#include <check.h>
#include <ooc/object.h>
#include <ooc/file.h>
#include "check_utils.h"

START_TEST(test_file_create){
    struct File* file = new(File, "test_files/test-1.txt");
    print(file);
    print(file_read(file, 100));
    del(file);
    }
END_TEST


Suite * file_suite_create (void) {
    Suite* s = suite_create("File");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_file_create);
    suite_add_tcase(s, tc_core);
    return s;
}



