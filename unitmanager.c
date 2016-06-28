#include "unitmanager.h"

#include "bson.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unitmanager *unitmanager_init()
{
    return malloc(sizeof(unitmanager));
}

unit *unitmanager_loadservice(unitmanager *unitman, const char *unit_path)
{
    printf("loading unit: %s\n", unit_path);

    unit *u = malloc(sizeof(unit));
    if (!u) {
        return NULL;
    }
    u->name = strdup(unit_path);

    if (string_ends_with(unit_path, ".service")) {
        u->type = UNIT_TYPE_SERVICE;
    } else if (string_ends_with(unit_path, ".mount")) {
        u->type = UNIT_TYPE_MOUNT;
    }

    unsigned int size;
    u->doc = map_file(unit_path, O_RDONLY | O_CLOEXEC, &u->fd, &size);

    return u;
}

void start_unit(unit *u)
{
    switch (u->type) {
        case UNIT_TYPE_SERVICE:
        break;

        case UNIT_TYPE_MOUNT:
        break;
    }
}

int ref_unit(unit *u)
{

}

int unref_unit(unit *u)
{

}
