#include <check.h>
#include <ooc/object.h>
#include <ooc/string.h>
#include <ooc/hashmap.h>
#include "check_utils.h"

START_TEST(test_string_create_delete){
    const struct String * s;
    s = new(String, "Hello");
    ck_assert_msg(s != NULL, "Failed to create Hello string");
    ck_assert_str_eq("Hello", c_str(s));
    del(s);

    s = new(String, NULL);
    ck_assert_msg(s == NULL, "String initialized with NULL created and was not NULL");

}
END_TEST

START_TEST(test_string_append){
    const struct String * h;
    const struct String * w;
    const struct String * s;
    h = new(String, "Hello");
    w = new(String, " World");
    ck_assert_msg(h != NULL, "New string is NULL");

    s = append(h, w);

    ck_assert_str_eq(c_str(h), "Hello");
    ck_assert_str_eq(c_str(w), " World");
    ck_assert_str_eq(c_str(s), "Hello World");
    ck_assert_msg(s != w && h !=w, "New string has same address as existing string");


}
END_TEST

START_TEST(test_string_equals){

    const struct String * h;
    const struct String * c;
    h = new(String, "Hello");

    ck_assert_msg(h != NULL, "New string is NULL");

    c= copy(h);

    // Should be equal even with hash method
    ck_assert(equals(h, h));

    // Copy should also be equal
    ck_assert(equals(h, c));
}
END_TEST

START_TEST(test_string_copy){

    const struct String * h;
    const struct String * c;
    h = new(String, "Hello");

    ck_assert_msg(h != NULL, "New string is NULL");

    c= copy(h);

    ck_assert_str_eq(c_str(h), c_str(c));
    ck_assert(h != c);

    del(h);
    // ensure copy persists when source is deleted
    ck_assert_str_eq(c_str(c), "Hello");

    del(c);

}
END_TEST

START_TEST(test_string_hash_simple){

    const struct String * h;
    const struct String * c;
    const struct String * n;

    h = new(String, "Hello");
    c = new(String, "Hello");
    n = new(String, "hello");

    ck_assert_str_eq(c_str(h), c_str(c));
    ck_assert(h != c);
    ck_assert(hash(h) == hash(c));
}
END_TEST

START_TEST(test_string_hash_append){

        const struct String* hello;
        const struct String* world;
        const struct String* space;
        const struct String* hello_world_constructed;
        const struct String* hello_world = new(String, "Hello world");
        hello = new(String, "Hello");
        space = new(String, " ");
        world = new(String, "world");

        hello_world_constructed = append(append(hello, space), world);

        ck_assert_str_eq(c_str(hello_world), c_str(hello_world_constructed));
        ck_assert(hash(hello_world) == hash(hello_world_constructed));
    }
END_TEST


START_TEST(test_string_hashing_many) {
    const size_t NUM_ITERATIONS = 100000;
    const struct String* keys[NUM_ITERATIONS];
    const struct String* values[NUM_ITERATIONS];
    const struct HashMap* hashmap = new(HashMap);

    for (size_t i = 0; i < NUM_ITERATIONS; i++) {
        char key[10];
        char value[10];
        fill_random_str(key, sizeof(value));
        //fill_random_str(value, sizeof(value));
        memcpy(value, key, sizeof(value));

        keys[i]  = new(String, key);
        values[i]  = new(String, value);
    }
    // Start timing at beginning of inserts
    clock_t t = clock();
    size_t collisions = 0;
    for (size_t i = 0; i < NUM_ITERATIONS; i++) {
        ck_assert_int_eq(i - collisions, len(hashmap));
        void* item;
        set_item(hashmap, keys[i], values[i]);

    }
    clock_t insert_clocks = clock() - t;
    t = clock();
    for (size_t i = 0; i < NUM_ITERATIONS; i++) {
        ck_assert_str_eq(c_str(values[i]), c_str(get_item(hashmap, keys[i])));
    }
    clock_t get_clocks = clock() - t;
    double insert_time = ((double)insert_clocks)/CLOCKS_PER_SEC; // in seconds
    double get_time = ((double)get_clocks)/CLOCKS_PER_SEC; // in seconds
    //printf("Inserted %zu items in %f seconds\n", NUM_ITERATIONS, insert_time);
    //printf("Retrieved %zu items in %f seconds\n", NUM_ITERATIONS, get_time);

    //printf("Detected %zu hash collisions\n", collisions);

    del(hashmap);

}
END_TEST

Suite * string_suite_create (void) {
    Suite * s = suite_create("String");
    TCase *tc_core = tcase_create("Core");
    TCase *tc_hashing = tcase_create("Hashing");

    tcase_add_test(tc_core, test_string_create_delete);
    tcase_add_test(tc_core, test_string_append);
    tcase_add_test(tc_core, test_string_copy);
    tcase_add_test(tc_core, test_string_equals);

    tcase_add_test(tc_hashing, test_string_hash_simple);
    tcase_add_test(tc_hashing, test_string_hash_append);
    tcase_add_test(tc_hashing, test_string_hashing_many);

    suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_hashing);
    return s;
}

