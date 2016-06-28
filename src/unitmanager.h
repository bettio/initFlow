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

#ifndef _UNITMANAGER_H_
#define _UNITMANAGER_H_

#define UNIT_TYPE_NULL 0
#define UNIT_TYPE_SERVICE 1
#define UNIT_TYPE_MOUNT 2

struct Unit
{
    char *name;
    void *doc;
    int type;
    int fd;
} typedef Unit;

struct unitmanager
{
} typedef unitmanager;

unitmanager *unitmanager_init();
Unit *unitmanager_loadservice(unitmanager *unitman, const char *unit_name);


void unit_constructor(Unit *u, const char *unit_path);
int unit_ref(Unit *u);
int unit_unref(Unit *u);

void unit_start(Unit *u);

#endif
