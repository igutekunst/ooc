#include <check.h>
#include <ooc/object.h>
#include <ooc/string.h>
#include <ooc/hashmap.h>
#include "check_utils.h"

START_TEST(test_hashmap_add_delete) {
        srandom(1337);
        const size_t NUM_ITERATIONS = 1000;
        const struct String* keys[NUM_ITERATIONS];
        const struct String* values[NUM_ITERATIONS];
        const struct HashMap* hashmap = new(HashMap);
        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            ck_assert_int_eq(i, len(hashmap));
            char key[10];
            char value[10];
            fill_random_str(key, sizeof(value));
            fill_random_str(value, sizeof(value));

            keys[i]  = new(String, key);
            values[i]  = new(String, value);
            set_item(hashmap, keys[i], values[i]);
        }
        ck_assert_int_eq(NUM_ITERATIONS, len(hashmap));

        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            ck_assert_str_eq(c_str(values[i]), c_str(get_item(hashmap, keys[i])));
        }

        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            ck_assert_int_eq(NUM_ITERATIONS -i, len(hashmap));
            del_item(hashmap, keys[i]);
        }

        //del(hashmap);

    }
END_TEST

Suite * hashmap_suite_create (void) {
    Suite * s = suite_create("HashMap");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_hashmap_add_delete);
    suite_add_tcase(s, tc_core);
    return s;
}

