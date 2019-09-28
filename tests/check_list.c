//
// Created by Isaac Gutekunst on 9/28/19.
//

#include "check_list.h"
//
// Created by Isaac Gutekunst on 9/27/19.
//

#include "check_list.h"
#include <check.h>
#include <ooc/object.h>
#include <ooc/list.h>
#include <ooc/string.h>
#include <ooc/int.h>
#include "check_utils.h"

START_TEST(test_list_create)
    {
        const void* list = new(List);
        ck_assert_int_ne(0, size(list));
        ck_assert_int_eq(0, len(list));
        ck_assert_str_eq("[]", str(list));

        del(list);
    }
END_TEST

START_TEST(test_list_append)
    {
        const void* list = new(List);
        append(list, new(String, "One"));
        ck_assert_int_eq(1, len(list));
        ck_assert_str_eq("[\"One\"]", str(list));

        append(list, new(String, "Two"));
        ck_assert_int_eq(2, len(list));
        ck_assert_str_eq("[\"One\", \"Two\"]", str(list));
    }
END_TEST

START_TEST(test_list_get)
    {
        const size_t NUM_ITERATIONS = 1000;
        struct String* strings[NUM_ITERATIONS];

        const void* list = new(List);

        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            char test_string[10];
            fill_random_str(test_string, sizeof(test_string));
            strings[i] = typed_new(String, test_string);

            ck_assert_int_eq(i, len(list));
            append(list, strings[i]);
        }

        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            struct String* string = get_item(list, new(Int, i));
            ck_assert(equals(string, strings[i]));
            del(strings[i]);
        }
        del(list);
    }
END_TEST

START_TEST(test_list_iterate)
    {
        const size_t NUM_ITERATIONS = 1000;
        struct String* strings[NUM_ITERATIONS];

        const void* list = new(List);

        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            char test_string[10];
            fill_random_str(test_string, sizeof(test_string));
            strings[i] = typed_new(String, test_string);

            ck_assert_int_eq(i, len(list));
            append(list, strings[i]);
        }

        const void* list_it = iter(list) ;
        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            struct String* string = next(list_it);
            ck_assert(equals(string, strings[i]));
            del(strings[i]);
        }
        del(list);
    }
END_TEST

START_TEST(test_list_get_wrong_index_type)
    {
        const void* list = new(List);
        //TODO should fail
        struct String* string = get_item(list, new(String, "asdf"));
    }
END_TEST


Suite* list_suite_create(void) {
    Suite* s = suite_create("List");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_list_create);
    tcase_add_test(tc_core, test_list_append);
    tcase_add_test(tc_core, test_list_get);
    tcase_add_test(tc_core, test_list_iterate);
    //tcase_add_exit_test(tc_core, test_list_get_wrong_index_type, 1);
    suite_add_tcase(s, tc_core);
    return s;
}


