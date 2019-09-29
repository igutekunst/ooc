cmake_minimum_required(VERSION 3.12)

set(CMAKE_MACOSX_RPATH 1)
project(ooc C CXX)

set(CMAKE_C_STANDARD 99)

link_directories(/usr/local/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(check REQUIRED)

add_library(ooc
        src/hashmap.c
        src/int.c
        src/iterator.c
        src/object.c
        src/person.c
        src/point.c
        src/string.c
        src/math.c

        include/ooc/person.h
        include/ooc/point.h
        include/ooc/string.h
        include/ooc/object.h
        include/ooc/int.h
        include/ooc/hashmap.h

        src/trait_math.h
        include/ooc/math.h
        src/list.c
        include/ooc/list.h
        include/ooc/object_va_args.h
        src/tuple.c
        include/ooc/tuple.h
        src/trait_file.c
        src/file.c
        src/trait_file.h
        include/ooc/file.h
        src/trait_string.c src/trait_string.h)

target_include_directories(ooc PRIVATE include)
add_executable(main src/main.c )
target_include_directories(main PRIVATE include)

target_link_libraries(main ooc)


add_executable(test_all
        tests/check_utils.c
        tests/check_utils.h
        tests/check_string.c
        tests/check_string.h
        tests/check_hashmap.c
        tests/check_hashmap.h
        tests/check_main.c
        tests/check_int.c
        tests/check_int.h
        tests/check_list.c
        tests/check_list.h
        tests/check_count_words.c tests/check_count_words.h tests/check_tuple.c tests/check_tuple.h tests/check_file.c tests/check_file.h)

LINK_DIRECTORIES(/usr/local/lib)

target_link_libraries(test_all ${CHECK_LIBRARIES} ooc)
target_include_directories(test_all PRIVATE ${CHECK_INCLUDE_DIRS})

target_include_directories(test_all PRIVATE include)
target_include_directories(test_all PRIVATE src)

add_executable(count_words
        examples/count_words.c
        )

target_include_directories(count_words PRIVATE include)
target_link_libraries(count_words ooc)


