/**
 * @file route.h
 * @brief .route units management functions.
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

#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "unitmanager.h"

/**
 * @brief instantiate a new Route unit.
 *
 * @description read a .route unit and instantiate a new Route object.
 *
 */
Unit *route_new(const char *route_new);

/**
 * @brief start the route.
 *
 * @description start the route pointed by u.
 *
 * @param u the route that should be started.
 */
int route_start(Unit *u);

#endif
