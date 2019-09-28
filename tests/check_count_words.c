//
// Created by Isaac Gutekunst on 2019-09-28.
//

#include "check_count_words.h"
#include <ooc/list.h>
#include <ooc/hashmap.h>
#include <ooc/math.h>
#include <ooc/int.h>
#include <ooc/string.h>
#include <ooc/object.h>
//#include <ooc/file.h>

/**
 *
 * This test shows a more complete usage of the OOC library to solve a common
 * interview question.
 *
 * Problem Statement: Given a file full of words, count the time every word occurs. For bunus
 * points, print the counts in sorted order.
 *
 * A word is defined as one or more non space characters. For now, only the space chracter needs
 * to be considered, but also treating tab a separator is even better.
 *
 * @param _i
 */
START_TEST(test_list_iterate)
    {
    /*
     * This solution is implemented using a hash table for performance.
     *
     * TODO load file
     *
     * The program operates one line at a time. It splits the line into
     * words using the space character, and then iterates over the words.
     *
     * If a word has not been seen before, a 0 is inserted into the HashMap using
     * the word as a key. Then the count is incremented.
     *
     * TODO: At the end, the totals are printed by looping over the HashTable keys.
     * TODO: The keys are sorted by getting a tuple for each (key,value) pair, and sorting the list
     *
     *
     */
        const struct List* list = new(List,
                                      new(String, "One"),
                                      new(String, "Two"),
                                      new(String, "Two"),
                                      new(String, "Three"));

        const struct HashMap* counts = new(HashMap);
        const void* it = iter(list);
        const struct String* string;
        for (string = next(it); string != NULL; string = next(it)) {
            const struct Int* count = NULL;
            if ((count = get_item(counts, string) ) == NULL) {
                count = new(Int, 0);
                set_item(counts, string, count);
            }
            set_item(counts, string, obj_add(count, new(Int, 1)));
            del(count);
        }

        const void * count_it = iter(counts);
        printf("\n");
        printf("|     Word       |  count  |\n");
        printf("+----------------+---------+\n");
        for (const void* key = next(count_it); key != NULL; key = next(count_it)) {

            printf("| %-14s | %-5d   |\n", c_str(key), obj_to_int(get_item(counts, key)));
            printf("+----------------+---------+\n");
        }
        printf("\n");
    }
END_TEST

Suite *counting_words_suite_create(void) {
    Suite *s = suite_create("Counting Example Suite");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_list_iterate);
    suite_add_tcase(s, tc_core);
    return s;
}

