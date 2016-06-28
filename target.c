#include "target.h"

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

static int check_is_unit_filename(const char *filename)
{
    return string_ends_with(filename, ".service") ||
           string_ends_with(filename, ".mount");
}

void target_append_unit(inittarget *t, unit *u)
{
    t->units[t->units_count] = u;
    t->units_count++;
}

inittarget *target_load(unitmanager *um, const char *path)
{
    inittarget *t = malloc(sizeof(inittarget));
    if (!t) {
        return NULL;
    }
    t->units_count = 0;

    DIR *targetDir = opendir(path);
    if (!targetDir) {
        printf("init: failed to open %s directory.\n", path);
        return NULL;
    }
    struct dirent dirEntry;
    struct dirent *entry = NULL;

    while ((readdir_r(targetDir, &dirEntry, &entry) == 0) && (entry != NULL)) {
        if (check_is_unit_filename(entry->d_name)) {
            char fullpath[256];
            strncpy(fullpath, path, 256);
            strncat(fullpath, entry->d_name, 256);
            unit *u = unitmanager_loadservice(um, fullpath);
            ref_unit(u);
            target_append_unit(t, u);
        }
    }

    return t;
}

int target_destroy(inittarget *t)
{
    free(t);
    return 0;
}

void target_start_all(inittarget *t)
{
    for (int i = 0; i < t->units_count; i++) {
        printf("starting %s.\n", t->units[i]->name);
    }
}
