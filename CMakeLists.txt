cmake_minimum_required(VERSION 2.8)
project(init)

option(BUILD_STATIC "Build static executable." OFF)

set(init_SRCS
    src/init.c
    src/bson.c
    src/eventloop.c
    src/interface.c
    src/mount.c
    src/unitmanager.c
    src/route.c
    src/service.c
    src/target.c
    src/utils.c
)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} --std=gnu99 -Wall -pedantic")
endif ()

if (BUILD_STATIC)
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBRARIES OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")
endif ()

add_executable(init ${init_SRCS})
