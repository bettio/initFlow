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

#include "mount.h"

#include "bson.h"
#include "utils.h"

#include <fcntl.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Mount
{
    Unit parent_instance;
    const char *what;
    const char *where;
    const char *type;
} Mount;

Unit *mount_new(const char *mount_path)
{
    Mount *new_mount = malloc(sizeof(Mount));
    if (!new_mount) {
        return NULL;
    }

    unit_constructor((Unit *) new_mount, mount_path);
    new_mount->parent_instance.type = UNIT_TYPE_MOUNT;

    unsigned int size;
    int fd;
    void *doc = map_file(mount_path, O_RDONLY | O_CLOEXEC, &fd, &size);

    if (!doc && (fd >= 0)) {
        free(new_mount);
        return NULL;
    }

    uint8_t type;
    const void *value = bson_key_lookup("what", doc, &type);
    new_mount->what = strdup(bson_value_to_string(value, NULL));

    value = bson_key_lookup("where", doc, &type);
    new_mount->where = strdup(bson_value_to_string(value, NULL));

    value = bson_key_lookup("type", doc, &type);
    new_mount->type = strdup(bson_value_to_string(value, NULL));

    munmap(doc, size);
    close(fd);

    return (Unit *) new_mount;
}

int mount_start(Unit *u)
{
    Mount *mnt = (Mount *) u;

    printf("starting mount %s.\n", u->name);
    if (mount(mnt->what, mnt->where, mnt->type, 0, NULL) < 0) {
        fprintf(stderr, "init: failed to mount %s on %s: ", mnt->what, mnt->where);
        perror("");
        return -1;
    }

    return 0;
}
