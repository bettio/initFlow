project(init)

set(init_SRCS
    init.c
    bson.c
    eventloop.c
    unitmanager.c
    target.c
    utils.c
)

add_executable(init ${init_SRCS})
