#include <check.h>
#include <ooc/object.h>
#include <ooc/string.h>

START_TEST(test_string_create_delete){
    const struct String * s;
    s = new(String, "Hello");
    ck_assert_msg(s != NULL, "Failed to create Hello string");
    ck_assert_str_eq("Hello", str(s));
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

    ck_assert_str_eq(str(h), "Hello");
    ck_assert_str_eq(str(w), " World");
    ck_assert_str_eq(str(s), "Hello World");
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

    ck_assert_str_eq(str(h), str(c));
    ck_assert(h != c);

    del(h);
    // ensure copy persists when source is deleted
    ck_assert_str_eq(str(c), "Hello");

    del(c);

}
END_TEST

START_TEST(test_string_hash){

    const struct String * h;
    const struct String * c;
    h = new(String, "Hello");

    c= copy(h);

    ck_assert_str_eq(str(h), str(c));
    ck_assert(h != c);
    
    //ck_assert(hash(h) == hash(c));

}
END_TEST



Suite * string_suite (void) {
    Suite * s = suite_create("String");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_string_create_delete);
    tcase_add_test(tc_core, test_string_append);
    tcase_add_test(tc_core, test_string_copy);
    tcase_add_test(tc_core, test_string_equals);
    tcase_add_test(tc_core, test_string_hash);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void){
    int number_failed;
    Suite *s = string_suite();
    SRunner *sr = srunner_create (s);
    srunner_run_all (sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
