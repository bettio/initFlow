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

#include "unit.h"

#include "bson.h"
#include "interface.h"
#include "mount.h"
#include "route.h"
#include "service.h"

#include <string.h>

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

void unit_constructor(Unit *u, const char *unit_path, void *doc)
{
    u->name = strdup(strrchr(unit_path, '/') + 1);
    u->status = UNIT_STATUS_INACTIVE;
    u->requires = ptr_list_new();
    u->dependency_status = 0;

    uint8_t type;
    const void *value = bson_key_lookup("requires", doc, &type);
    if (value && (type == TYPE_STRING)) {
        ptr_list_append(u->requires, strdup(bson_value_to_string(value, NULL)));
    }
}

int unit_get_status(const Unit *u)
{
    return u->status;
}

void unit_set_status(Unit *u, int status)
{
    u->status = status;
}
