#include <check.h>
#include <ooc/object.h>
#include <ooc/string.h>
#include <ooc/hashmap.h>
#include "check_utils.h"

START_TEST(test_hashmap_add_delete) {
        srandom(1337);
        const size_t NUM_ITERATIONS = 10;
        const struct String* keys[NUM_ITERATIONS];
        const struct String* values[NUM_ITERATIONS];
        const struct HashMap* hashmap = new(HashMap);
        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            ck_assert_int_eq(i, len(hashmap));
            char key[10];
            char value[10];
            fill_random_chars(key, sizeof(value));
            fill_random_chars(value, sizeof(value));

            keys[i]  = new(String, key);
            values[i]  = new(String, value);
            insert(hashmap, keys[i], values[i]);
        }
        ck_assert_int_eq(NUM_ITERATIONS, len(hashmap));

        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            ck_assert_str_eq(str(values[i]), str(get_item(hashmap, keys[i])));
        }

        for (size_t i = 0; i < NUM_ITERATIONS; i++) {
            printf("before delete: len: %d, NI: %d\n", len(hashmap), NUM_ITERATIONS - i );
            ck_assert_int_eq(NUM_ITERATIONS -i, len(hashmap));
            del_item(hashmap, keys[i]);
            printf("after delete: len: %d, NI: %d\n", len(hashmap), NUM_ITERATIONS - i );
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

