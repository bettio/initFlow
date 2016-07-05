/***************************************************************************
 *   Copyright 2016 by Davide Bettio <davide@uninstall.it>                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "service.h"

#include "bson.h"
#include "eventloop.h"
#include "unitmanager.h"
#include "utils.h"

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
    Unit parent_instance;
    const char *exec;
    pid_t pid;
    int exit_status;
    int error_number;
    int restart;
} Service;

Unit *service_new(const char *service_path)
{
    Service *new_service = malloc(sizeof(Service));
    if (!new_service) {
        return NULL;
    }
    new_service->pid = 0;
    new_service->exit_status = 0;
    new_service->error_number = 0;

    unsigned int size;
    int fd;
    void *doc = map_file(service_path, O_RDONLY | O_CLOEXEC, &fd, &size);

    if (!doc && (fd >= 0)) {
        free(new_service);
        return NULL;
    }

    unit_constructor((Unit *) new_service, service_path, doc);
    new_service->parent_instance.type = UNIT_TYPE_SERVICE;

    uint8_t type;
    const void *value = bson_key_lookup("exec", doc, &type);
    if (!value) {
        fprintf(stderr, "init: cannot execute %s: missing exec key\n", new_service->parent_instance.name);
        free(new_service);
        return NULL;
    }
    new_service->exec = strdup(bson_value_to_string(value, NULL));

    value = bson_key_lookup("restart", doc, &type);
    new_service->restart = value && (strcmp(bson_value_to_string(value, NULL), "always") == 0);

    munmap(doc, size);
    close(fd);

    return (Unit *) new_service;
}

int service_start(Unit *u)
{
    Service *srv = (Service *) u;

    printf("Starting service: %s.\n", u->name);

    pid_t pid = fork();
    if (!pid){
        char *a[] = {(char *) srv->exec, NULL};
        if (execv(srv->exec, a)) {
            fprintf(stderr, "init: cannot execute: %s: ", srv->exec);
            perror("");
            _exit(EXIT_FAILURE);
        }
    } else if (pid > 0) {
        unit_set_status(u, UNIT_STATUS_RUNNING);
        srv->pid = pid;
        event_loop_add_child(pid, service_process_event, srv);
    } else {
        fprintf(stderr, "init: cannot fork: %s: ", srv->exec);
        perror("");
        unit_set_status(u, UNIT_STATUS_FAILED);
    }

    return 0;
}

static void service_process_event(pid_t pid, void *userdata)
{
    Service *srv = (Service *) userdata;

    unit_set_status((Unit *) srv, UNIT_STATUS_TERMINATED);

    if (srv->restart) {
        printf("init: %i has terminated. restarting: %s.\n", pid, srv->parent_instance.name);
        service_start((Unit *) srv);
    }
}
