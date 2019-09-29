//
// Created by Isaac Gutekunst on 2019-09-28.
//

#include "check_file.h"
#include <ooc/string.h>
#include <check.h>
#include <ooc/object.h>
#include <ooc/file.h>
#include <struct.h>
#include "check_utils.h"
#include "object_internal.h"

START_TEST(test_file_create)
    {
        const struct File* file = new(File, "test_files/test-1.txt");
        const struct String* contents = file_read(file);
        ck_assert_int_eq(85, len(contents));
        ck_assert_int_eq(85, len(file));
        del(file);
        del(contents);
    }
END_TEST

START_TEST(test_file_read_line)
    {
        const struct File* file = new(File, "test_files/test-2.txt");
        const struct String* expected[4] = {
                new(String, "one\n"),
                new(String, "two\n"),
                new(String, "three\n"),
                new(String, "four\n")
        };



        for (size_t i = 0; i < COUNTOF(expected); i++) {
            const struct String* line = file_read_line(file);
            ck_assert_str_eq(c_str(expected[i]), c_str(line));
            del(line);
            del(expected[i]);
        }
        del(file);
    }

END_TEST

START_TEST(test_file_empty_line)
    {
        const struct File* file = new(File, "test_files/test-one-newline.txt");
        const struct String* line = file_read_line(file);
        ck_assert(line);
        ck_assert_str_eq("\n", c_str(line));
        ck_assert_int_eq(1, len(line));
        del(file);
    }

END_TEST


Suite* file_suite_create(void) {
    Suite* s = suite_create("File");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_file_create);
    tcase_add_test(tc_core, test_file_read_line);
    tcase_add_test(tc_core, test_file_empty_line);
    suite_add_tcase(s, tc_core);
    return s;
}

//TODO add test for EOF (read_line should return NULL)




