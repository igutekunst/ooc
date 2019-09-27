cmake_minimum_required(VERSION 3.12)

set(CMAKE_MACOSX_RPATH 1)
project(ooc C CXX)

set(CMAKE_C_STANDARD 99)

link_directories(/usr/local/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(check REQUIRED)

add_library(ooc
        src/hashmap.c
        src/Int.c
        src/iterator.c
        src/object.c
        src/person.c
        src/point.c
        src/string.c
        src/trait_math.c

        include/ooc/person.h
        include/ooc/point.h
        include/ooc/string.h
        include/ooc/object.h
        include/ooc/Int.h
        include/ooc/hashmap.h

        src/trait_math_internal.h
        include/ooc/trait_math.h
        )

target_include_directories(ooc PRIVATE include)
add_executable(main src/main.c )
target_include_directories(main PRIVATE include)

target_link_libraries(main ooc)


add_executable(test_string
        tests/check_utils.c
        tests/check_utils.h
        tests/check_string.c
        tests/check_string.h
        tests/check_hashmap.c
        tests/check_hashmap.h
        tests/check_main.c
        tests/check_int.c
        tests/check_int.h
        )

LINK_DIRECTORIES(/usr/local/lib)

target_link_libraries(test_string ${CHECK_LIBRARIES} ooc)
target_include_directories(test_string PRIVATE ${CHECK_INCLUDE_DIRS})

target_include_directories(test_string PRIVATE include)


