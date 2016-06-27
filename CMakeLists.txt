project(init)

set(init_SRCS
    "init.c"
    "unitmanager.c"
    "target.c"
)

add_executable(init ${init_SRCS})
