/**
 * @file target.h
 * @brief target management functions.
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

#ifndef _TARGET_H_
#define _TARGET_H_

#include "unitmanager.h"

struct _Target typedef Target;

/**
 * @brief load a target.
 *
 * @description load a target and all the associated units.
 *
 * @param um the UnitManager.
 * @param path to the target wants directory.
 */
Target *target_load(UnitManager *um, const char *path);

/**
 * @brief destory a target.
 *
 * @description destroy a target and free its allocated memory.
 */
void target_destroy(Target *t);

/**
 * @brief start all target units
 *
 * @description start all required units for a certain target.
 *
 * @param t target that should be started.
 */
void target_start_all(Target *t);

#endif
