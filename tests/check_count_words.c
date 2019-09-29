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
#include <ooc/tuple.h>
#include <ooc/file.h>
#include <object_internal.h>


/**
 *
 * This test shows a more complete usage of the OOC library to solve a common
 * interview question.
 *
 * Problem Statement: Given a file full of words, count the time every word occurs. For bonus
 * points, print the counts in sorted order.
 *
 * A word is defined as one or more non space characters. For now, only the space character needs
 * to be considered, but also treating tab a separator is even better.
 *
 * @param _i
 */
START_TEST(test_list_iterate)
    {
        /*
         * The general idea here is to store the count of all seen words in a HashMap, and
         * then sort the results later for printing
         * First we open a file, and read on line at a time
         *
         * Then we split the line into words using the space character,
         * and then iterates over the words.
         *
         * If a word has not been seen before, a 0 is inserted into the HashMap using
         * the word as a key. Then the count is incremented.
         *
         * At the end, the totals are printed by looping over the HashTable keys.
         * The keys are sorted by getting a tuple for each (key,value) pair, and sorting the list
         *
         * Note: The ooc library does not support referencing counting, and as such, allocates
         * lots of memory that we don't free. Notable examples are temporary variables used
         * to make calculations, and because almost all ooc functions require ooc objects as arguments.
         *
         * In the future, referencing counting will make avoiding memory leaks much easier.
         */

        const bool print_counts = true;

        const struct HashMap* counts = new(HashMap);
        const struct File* f = new(File, "test_files/test-3.txt");

        const struct String* full_line = file_read_line(f);


        while (full_line != NULL) {

            // Remove trailing newline
            const struct String* line = string_slice(full_line, 0, -1);
            del(full_line);

            const struct List* list = string_split(line);

            // Iterate over the list of words, inserting them into the
            // HashMap
            const void* it = iter(list);
            const struct String* string;
            for (string = next(it); string != NULL; string = next(it)) {
                const struct Int* temp_count = NULL;
                if ((temp_count = get_item(counts, string)) == NULL) {
                    temp_count = new(Int, 0);
                    set_item(counts, string, temp_count);
                }
                set_item(counts, string, obj_add(temp_count, new(Int, 1)));
                del(temp_count);
            }
            del(list);
            full_line = file_read_line(f);
        }



        // Put the results into a List of Tuples for sorting
        const void* count_it = iter(counts);

        const struct List* counts_list = new(List);

        for (const void* key = next(count_it); key != NULL; key = next(count_it)) {
            const struct Tuple* t = new(Tuple, get_item(counts, key), key);
            append(counts_list, t);
        }
        obj_sort(counts_list, SORT_DESCENDING);


        // Iterate over the tuples for printing
        const void* ci = iter(counts_list);
        const struct Tuple* t;


        const struct String* words[] = {
                new(String, "one"),
                new(String, "two"),
                new(String, "three"),
                new(String, "four"),
                new(String, "five"),
        };

        for (size_t i = 0; i < COUNTOF(words); i++) {
            ck_assert_int_eq(i + 1, obj_to_int(get_item(counts, words[i])));
            del(words[i]);
        }

        if (print_counts) {
            for (t = next(ci); t != NULL; t = next(ci)) {
                // TODO consider making a integer only index function similar to get_item
                printf(" %d, %-14s\n", obj_to_int(get_item(t, new(Int, 0))), c_str(get_item(t, new(Int, 1))) );
            }
            printf("\n");
        }


        // Note: some memory still leaks, notably temporary Ints.
        // This will be fixed in future versions
        del(count_it);
        del(ci);
        del(counts_list);
        del(counts);
        del(f);

    }
END_TEST

Suite* counting_words_suite_create(void) {
    Suite* s = suite_create("Counting Example Suite");
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_list_iterate);
    suite_add_tcase(s, tc_core);
    return s;
}

