/**
 * @file unitmanager.h
 * @brief function to manage a set of units
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

#ifndef _UNITMANAGER_H_
#define _UNITMANAGER_H_

#include "unit.h"

struct _UnitManager typedef UnitManager;

/**
 * @brief initialize unitmanager
 *
 * @return newly created UnitManager.
 */
UnitManager *unitmanager_init();

/**
 * @brief load any unit file
 *
 * @description load any unit file (.unit, .service, .mount, etc...) and istantiates them by
   calling the right constructor.
 */
Unit *unitmanager_loadunit(UnitManager *unitman, const char *unit_name);


void unitmanager_build_list(UnitManager *unitman);

#endif
