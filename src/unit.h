/**
 * @file unit.h
 * @brief Unit related functions
 */

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

#ifndef _UNIT_H_
#define _UNIT_H_

#include "ptrlist.h"

#define UNIT_TYPE_NULL 0
#define UNIT_TYPE_TARGET 1
#define UNIT_TYPE_SERVICE 2
#define UNIT_TYPE_MOUNT 3
#define UNIT_TYPE_INTERFACE 4
#define UNIT_TYPE_ROUTE 5

/** @brief unit is not running and it is not scheduled */
#define UNIT_STATUS_INACTIVE 0
/** @brief unit is going to be executed soon */
#define UNIT_STATUS_SCHEDULED 1
/** @brief unit is running */
#define UNIT_STATUS_RUNNING 2
/** @brief unit has terminated without errors */
#define UNIT_STATUS_TERMINATED 3
/** @brief unit has terminated due to some errors */
#define UNIT_STATUS_FAILED 4

struct Unit
{
    char *name;
    int type;
    int status;
    int dependency_status;
    PtrList *requires;
} typedef Unit;

/**
 * @brief Unit constructor method.
 *
 * @details initialize an already allocated Unit based object.
 * @param unit the pointer to the uninitialized Unit based object.
 * @param unit_path the path to the .unit BSON file.
 */
void unit_constructor(Unit *unit, const char *unit_path, void *doc);

/**
 * @brief starts a Unit.
 */
void unit_start(Unit *u);

/**
 * @brief get Unit status.
 */
int unit_get_status(const Unit *u);

/**
 * @brief set Unit status.
 */
void unit_set_status(Unit *u, int status);

#endif
