#include "unitmanager.h"

#include "bson.h"
#include "mount.h"
#include "service.h"
#include "utils.h"

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
    unit *new_unit;

    if (string_ends_with(unit_path, ".service")) {
        new_unit = service_new(unit_path);

    } else if (string_ends_with(unit_path, ".mount")) {
        new_unit = mount_new(unit_path);
    }
    if (!new_unit) {
        return NULL;
    }

    return new_unit;
}

void unit_start(unit *u)
{
    switch (u->type) {
        case UNIT_TYPE_SERVICE:
            service_start(u);
        break;

        case UNIT_TYPE_MOUNT:
            mount_start(u);
        break;
    }
}

void unit_constructor(unit *u, const char *unit_path)
{
    u->name = strdup(unit_path);
}

int unit_ref(unit *u)
{

}

int unit_unref(unit *u)
{

}
