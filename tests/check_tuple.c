//
// Created by Isaac Gutekunst on 2019-09-28.
//

#include "check_tuple.h"
#include <check.h>
#include <ooc/object.h>
#include <ooc/tuple.h>
#include <ooc/string.h>
#include <ooc/int.h>
#include <ooc/list.h>

#include "check_utils.h"

START_TEST(test_tuple_create)
    {
        const void* tuple = new(Tuple);
        ck_assert_int_ne(0, size(tuple));
        ck_assert_int_eq(0, len(tuple));
        ck_assert_str_eq("()", c_str(tuple));

        del(tuple);
    }
END_TEST

START_TEST(test_tuple_create_va)
    {
        const struct Int* key_1 = new(Int, 0);
        const struct Int* key_2 = new(Int, 1);
        const struct Int* val_1 = new(Int, 100);
        const struct Int* val_2 = new(Int, 500);

        const void* tuple = new(Tuple, val_1, val_2);
        ck_assert_int_ne(2, size(tuple));
        ck_assert(equals(val_1, get_item(tuple, key_1)));
        ck_assert(equals(val_2, get_item(tuple, key_2)));
        ck_assert_str_eq("(100, 500)", c_str(tuple));

        del(tuple);
        del(key_1);
        del(key_2);
        del(val_1);
        del(val_2);
    }
END_TEST

START_TEST(test_tuple_create_va_mixed)
    {
        const struct Int* key_1 = new(Int, 0);
        const struct Int* key_2 = new(Int, 1);
        const struct String* val_1 = new(String, "Zero");
        const struct String* val_2 = new(String, "One");

        const void* tuple = new(Tuple, val_1, val_2);
        ck_assert_int_ne(2, size(tuple));
        ck_assert(equals(val_1, get_item(tuple, key_1)));
        ck_assert(equals(val_2, get_item(tuple, key_2)));
        ck_assert_str_eq("(Zero, One)", c_str(tuple));

        del(tuple);
        del(key_1);
        del(key_2);
        del(val_1);
        del(val_2);
    }
END_TEST


START_TEST(test_tuple_sorting_list)
    {
        const struct Tuple* t1 = new(Tuple, new(Int, 1), new(String, "One"));
        const struct Tuple* t2 = new(Tuple, new(Int, 2), new(String, "Two"));
        const struct Tuple* t3 = new(Tuple, new(Int, 3), new(String, "Three"));
        const struct Tuple* t4 = new(Tuple, new(Int, 4), new(String, "Four"));


        const struct List* test = new(List, t3, t2, t1, t4);
        // TODO finish sorting test
        //print(test);
        obj_sort(test);
        //print(test);
    }
END_TEST


Suite* tuple_suite_create(void) {
    Suite* s = suite_create("List");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_tuple_create);
    tcase_add_test(tc_core, test_tuple_create_va);
    tcase_add_test(tc_core, test_tuple_create_va_mixed);
    tcase_add_test(tc_core, test_tuple_sorting_list);
    //tcase_add_exit_test(tc_core, test_list_get_wrong_index_type, 1);
    suite_add_tcase(s, tc_core);
    return s;
}



