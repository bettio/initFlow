project(init)

set(init_SRCS
    src/init.c
    src/bson.c
    src/eventloop.c
    src/mount.c
    src/unitmanager.c
    src/service.c
    src/target.c
    src/utils.c
)

add_executable(init ${init_SRCS})
