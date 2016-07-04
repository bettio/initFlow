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
#include "unit.h"
#include "utils.h"
#include "target.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NODE_CLEAN 0
#define NODE_TEMPORARY_MARK 1
#define NODE_PERMANENT_MARK 2

struct _UnitManager
{
    PtrList *units_list;
};

UnitManager *unitmanager_init()
{
    UnitManager *umanager = malloc(sizeof(UnitManager));
    if (!umanager) {
       return NULL;
    }
    umanager->units_list = ptr_list_new();
    if (!umanager->units_list) {
        free(umanager);
        return NULL;
    }

    return umanager;
}

Unit *unitmanager_loadunit(UnitManager *unitman, const char *unit_path)
{
    DEBUG_MSG("loading unit: %s\n", unit_path);
    Unit *new_unit;

    if (string_ends_with(unit_path, ".target")) {
        new_unit = target_new(unit_path, unitman);

    } else if (string_ends_with(unit_path, ".service")) {
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

    ptr_list_append(unitman->units_list, new_unit);

    return new_unit;
}

Unit *unitmanager_get_unit_by_name(UnitManager *unitman, const char *name)
{
    int count = ptr_list_count(unitman->units_list);

    for (int i = 0; i < count; i++) {
        Unit *u = ptr_list_at(unitman->units_list, i);
        if (!strcmp(name, u->name)) {
            return u;
        }
    }

    return NULL;
}

void visit_nodes_and_build(UnitManager *unitman, PtrList *unit, PtrList *deps_list, Unit *u)
{
    if (u->dependency_status == NODE_TEMPORARY_MARK) {
        fprintf(stderr, "Found circular dependency, giving up.\n");
        abort();
    } else if (u->dependency_status == NODE_CLEAN) {
        u->dependency_status = NODE_TEMPORARY_MARK;
        int deps_count = ptr_list_count(u->requires);
        for (int i = 0; i < deps_count; i++) {
            visit_nodes_and_build(unitman, unit, deps_list, unitmanager_get_unit_by_name(unitman, ptr_list_at(u->requires, i)));
        }
        u->dependency_status = NODE_PERMANENT_MARK;
        ptr_list_append(deps_list, u);
    }
}

static PtrList *build_dependencies_list(UnitManager *unitman, PtrList *units)
{
    PtrList *deps_list = ptr_list_new();
    int count = ptr_list_count(units);

    for (int i = 0; i < count; i++) {
        Unit *u = ptr_list_at(units, i);
        if (u->dependency_status == NODE_CLEAN) {
            visit_nodes_and_build(unitman, units, deps_list, u);
        }
    }

    return deps_list;
}

PtrList *unitmanager_build_dependencies_list(UnitManager *unitman)
{
    return build_dependencies_list(unitman, unitman->units_list);
}

void unitmanager_start_all(PtrList *list)
{
    int units_count = ptr_list_count(list);

    for (int i = 0; i < units_count; i++) {
        Unit *u = ptr_list_at(list, i);
        unit_set_status(u, UNIT_STATUS_SCHEDULED);
        unit_start(u);
    }
}
