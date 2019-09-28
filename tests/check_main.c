#include <check.h>
#include <stdlib.h>
#include "check_hashmap.h"
#include "check_string.h"
#include "check_int.h"
#include "check_list.h"

int main(void){
    int number_failed;
    Suite* ooc_suite = string_suite_create();
    Suite* hashmap_suite = hashmap_suite_create();
    Suite* int_suite = int_suite_create();
    Suite* list_suite = list_suite_create();

    SRunner *sr = srunner_create (ooc_suite);
    srunner_add_suite(sr, hashmap_suite);
    srunner_add_suite(sr, int_suite);
    srunner_add_suite(sr, list_suite);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all (sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
