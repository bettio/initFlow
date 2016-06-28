#include "mount.h"

#include "bson.h"

#include <fcntl.h>
#include <sys/mount.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Mount
{
    unit parent_instance;
    const char *what;
    const char *where;
    const char *type;
} Mount;

unit *mount_new(const char *mount_path)
{
    Mount *new_mount = malloc(sizeof(Mount));
    if (!new_mount) {
        return NULL;
    }

    unit_constructor((unit *) new_mount, mount_path);
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

    return (unit *) new_mount;
}

int mount_start(unit *u)
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
