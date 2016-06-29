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

#include "unitmanager.h"

#include "bson.h"
#include "interface.h"
#include "mount.h"
#include "route.h"
#include "service.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct _unitmanager
{
    Unit *units[256];
};

unitmanager *unitmanager_init()
{
    return malloc(sizeof(unitmanager));
}

Unit *unitmanager_loadunit(unitmanager *unitman, const char *unit_path)
{
    printf("loading unit: %s\n", unit_path);
    Unit *new_unit;

    if (string_ends_with(unit_path, ".service")) {
        new_unit = service_new(unit_path);

    } else if (string_ends_with(unit_path, ".mount")) {
        new_unit = mount_new(unit_path);

    } else if (string_ends_with(unit_path, ".interface")) {
        new_unit = interface_new(unit_path);

    } else if (string_ends_with(unit_path, ".route")) {
        new_unit = route_new(unit_path);
    }
    if (!new_unit) {
        return NULL;
    }

    return new_unit;
}

void unit_start(Unit *u)
{
    switch (u->type) {
        case UNIT_TYPE_SERVICE:
            service_start(u);
        break;

        case UNIT_TYPE_MOUNT:
            mount_start(u);
        break;

        case UNIT_TYPE_INTERFACE:
            interface_start(u);
        break;

        case UNIT_TYPE_ROUTE:
            route_start(u);
        break;
    }
}

void unit_constructor(Unit *u, const char *unit_path)
{
    u->name = strdup(strrchr(unit_path, '/') + 1);
}

void unit_ref(Unit *u)
{
}

void unit_unref(Unit *u)
{
}
