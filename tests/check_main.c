#include <check.h>
#include <stdlib.h>
#include "check_hashmap.h"
#include "check_string.h"

int main(void){
    int number_failed;
    Suite *string_suite = string_suite_create();
    Suite *hashmap_suite = hashmap_suite_create();
    SRunner *sr = srunner_create (string_suite);
    srunner_add_suite(sr, hashmap_suite);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all (sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
