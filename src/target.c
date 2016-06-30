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

#include "unitmanager.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

struct _Target
{
    int units_count;
    Unit *units[256];
};

static int check_is_unit_filename(const char *filename)
{
    return string_ends_with(filename, ".service") ||
           string_ends_with(filename, ".interface") ||
           string_ends_with(filename, ".route") ||
           string_ends_with(filename, ".mount");
}

void target_append_unit(Target *t, Unit *u)
{
    t->units[t->units_count] = u;
    t->units_count++;
}

Target *target_load(UnitManager *um, const char *path)
{
    Target *t = malloc(sizeof(Target));
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
            Unit *u = unitmanager_loadunit(um, fullpath);
            if (!u) {
                fprintf(stderr, "skipping unit %s\n", entry->d_name);
                continue;
            }
            target_append_unit(t, u);
        }
    }

    return t;
}

int target_destroy(Target *t)
{
    free(t);
    return 0;
}

void target_start_all(Target *t)
{
    for (int i = 0; i < t->units_count; i++) {
        unit_set_status(t->units[i], UNIT_STATUS_SCHEDULED);
        unit_start(t->units[i]);
    }
}
