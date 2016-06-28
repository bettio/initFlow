#include "eventloop.h"
#include "unitmanager.h"
#include "target.h"

#include <stdio.h>
#include <stdlib.h>

#define BASE_PREFIX ""
#define SYSTEM_START_TARGET_PATH BASE_PREFIX "system/targets/system-start.target/"

int main(int argc, char **argv, char **envp)
{
    printf("\n\ninit 0.1 - Starting system...\n\n");

    unitmanager *unitman = unitmanager_init();
    if (!unitman) {
        fprintf(stderr, "init: failed to initialize unit manager.\n");
        return EXIT_FAILURE;
    }

    inittarget *target = target_load(unitman, SYSTEM_START_TARGET_PATH);
    if (!target) {
        fprintf(stderr, "init: failed to load target: %s.\n", SYSTEM_START_TARGET_PATH);
        return EXIT_FAILURE;
    }

    target_start_all(target);

    EventLoop *loop = event_loop_new();
    return event_loop_run(loop);
}
