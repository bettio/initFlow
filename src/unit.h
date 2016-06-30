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

#define UNIT_TYPE_NULL 0
#define UNIT_TYPE_SERVICE 1
#define UNIT_TYPE_MOUNT 2
#define UNIT_TYPE_INTERFACE 3
#define UNIT_TYPE_ROUTE 4

#define UNIT_STATUS_INACTIVE 0
#define UNIT_STATUS_SCHEDULED 1
#define UNIT_STATUS_RUNNING 2
#define UNIT_STATUS_TERMINATED 3
#define UNIT_STATUS_FAILED 4

struct Unit
{
    char *name;
    void *doc;
    int type;
    int status;
} typedef Unit;

/**
 * @brief Unit constructor method.
 *
 * @details initialize an already allocated Unit based object.
 * @param unit the pointer to the uninitialized Unit based object.
 * @param unit_path the path to the .unit BSON file.
 */
void unit_constructor(Unit *unit, const char *unit_path);

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
