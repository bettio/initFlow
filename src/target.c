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

#include "target.h"

#include "config.h"
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
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

struct _Target
{
    Unit parent_instance;
};

static int check_is_unit_filename(const char *filename)
{
    return string_ends_with(filename, ".target") ||
           string_ends_with(filename, ".service") ||
           string_ends_with(filename, ".interface") ||
           string_ends_with(filename, ".route") ||
           string_ends_with(filename, ".mount");
}

Unit *target_new(const char *target_path, UnitManager *um)
{
    Target *new_target = malloc(sizeof(Target));
    if (!new_target) {
        return NULL;
    }

    unsigned int size;
    int fd;
    void *doc = map_file(target_path, O_RDONLY | O_CLOEXEC, &fd, &size);

    if (!doc && (fd >= 0)) {
        free(new_target);
        return NULL;
    }

    unit_constructor((Unit *) new_target, target_path, doc);
    new_target->parent_instance.type = UNIT_TYPE_TARGET;

    char requiresPath[256];
    sprintf(requiresPath, SYSTEM_TARGETS_PATH "%s.requires/", new_target->parent_instance.name);

    DIR *targetDir = opendir(requiresPath);
    if (!targetDir) {
        printf("init: failed to open %s directory.\n", requiresPath);
        return NULL;
    }
    struct dirent dirEntry;
    struct dirent *entry = NULL;

    while ((readdir_r(targetDir, &dirEntry, &entry) == 0) && (entry != NULL)) {
        if (check_is_unit_filename(entry->d_name)) {
            char fullpath[256];
            strncpy(fullpath, requiresPath, 256);
            strncat(fullpath, entry->d_name, 256);
            Unit *u = unitmanager_loadunit(um, fullpath);
            if (!u) {
                fprintf(stderr, "skipping unit %s\n", entry->d_name);
                continue;
            }
            ptr_list_append(new_target->parent_instance.requires, u->name);
        }
    }
    closedir(targetDir);

    munmap(doc, size);
    close(fd);

    return (Unit *) new_target;
}

void target_destroy(Target *t)
{
    free(t);
}

void target_start(Unit *u)
{
    printf("started target: %s\n", u->name);
}
