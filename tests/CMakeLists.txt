cmake_minimum_required(VERSION 2.8)
project(tests)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} --std=gnu99 -Wall -pedantic")
endif ()

add_executable(bson_test ../src/bson.c ../src/utils.c bson_test.c)
