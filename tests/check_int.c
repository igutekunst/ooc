//
// Created by Isaac Gutekunst on 9/27/19.
//

#include "check_int.h"
#include <check.h>
#include <ooc/int.h>
#include <ooc/math.h>
#include <ooc/object.h>
#include "check_utils.h"

START_TEST(test_int_create){
    const void* five = new(Int, 5);
    const void* two_thousand = new(Int, 2000);
    ck_assert_str_eq("5", str(five));

    const void* two_thousand_five = obj_add(five, two_thousand);

    ck_assert_str_eq("2005", str(two_thousand_five));
    ck_assert_int_eq(2005,obj_to_int(two_thousand_five));

    del(five);
    del(two_thousand);
    del(two_thousand_five);
}
END_TEST


Suite * int_suite_create (void) {
    Suite* s = suite_create("Int");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_int_create);
    suite_add_tcase(s, tc_core);
    return s;
}


