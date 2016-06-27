project(init)

set(init_SRCS
    "init.c"
    "bson.c"
    "unitmanager.c"
    "target.c"
)

add_executable(init ${init_SRCS})
