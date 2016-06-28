project(init)

set(init_SRCS
    init.c
    bson.c
    eventloop.c
    mount.c
    unitmanager.c
    service.c
    target.c
    utils.c
)

add_executable(init ${init_SRCS})
