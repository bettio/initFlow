#include "service.h"

#include "bson.h"
#include "eventloop.h"
#include "unitmanager.h"

#include <fcntl.h>
#include <sys/mount.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void service_process_event(pid_t pid, void *userdata);

typedef struct Service
{
    unit parent_instance;
    const char *exec;
    pid_t pid;
    int exit_status;
    int error_number;
    int restart;
} Service;

unit *service_new(const char *service_path)
{
    Service *new_service = malloc(sizeof(Service));
    if (!new_service) {
        return NULL;
    }
    new_service->pid = 0;
    new_service->exit_status = 0;
    new_service->error_number = 0;

    unit_constructor((unit *) new_service, service_path);
    new_service->parent_instance.type = UNIT_TYPE_SERVICE;

    unsigned int size;
    int fd;
    void *doc = map_file(service_path, O_RDONLY | O_CLOEXEC, &fd, &size);

    if (!doc && (fd >= 0)) {
        free(new_service);
        return NULL;
    }

    uint8_t type;
    const void *value = bson_key_lookup("exec", doc, &type);
    new_service->exec = strdup(bson_value_to_string(value, NULL));

    value = bson_key_lookup("restart", doc, &type);
    new_service->restart = value && (strcmp(bson_value_to_string(value, NULL), "always") == 0);
    return (unit *) new_service;
}

int service_start(unit *u)
{
    Service *srv = (Service *) u;

    printf("starting service %s.\n", u->name);

    pid_t pid = fork();
    if (!pid){
        char *envp[] = { NULL };
        char *a[] = {(char *) srv->exec, NULL};
        if (execve(srv->exec, a, envp)) {
            fprintf(stderr, "init: cannot execute: %s: ", srv->exec);
            perror("");
            _exit(EXIT_FAILURE);
        }
    } else {
        srv->pid = pid;
        unit_ref((unit *) srv);
        event_loop_add_child(pid, service_process_event, srv);
    }

    return 0;
}

static void service_process_event(pid_t pid, void *userdata)
{
    Service *srv = (Service *) userdata;
    printf("event has appened for %s (pid: %i)\n", srv->parent_instance.name, pid);

    if (srv->restart) {
        printf("restarting %s\n", srv->parent_instance.name);
        service_start((unit *) srv);
    }
}
