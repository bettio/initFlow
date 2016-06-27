#include "target.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

static int string_ends_with(const char *string, const char *end)
{
    int stringlen = strlen(string);
    int endlen = strlen(end);

    if (stringlen < endlen) {
        return 0;
    }

    return !strcmp(string + (stringlen - endlen), end);
}

static int check_is_unit_filename(const char *filename)
{
    return string_ends_with(filename, ".service") ||
           string_ends_with(filename, ".mount");
}

inittarget *target_load(unitmanager *um, const char *path)
{
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
            unitmanager_loadservice(um, fullpath);
        }
    }

    return malloc(sizeof(inittarget));
}

int target_destroy(inittarget *t)
{
    free(t);
    return 0;
}
