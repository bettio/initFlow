/**
 * @file service.h
 * @brief .service units management functions.
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

#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "unitmanager.h"

/**
 * @brief instantiate a new Service unit.
 *
 * @description read a .service unit and instantiate a new Service object.
 *
 */
Unit *service_new(const char *service_path);

/**
 * @brief start the service.
 *
 * @description start the service pointed by u.
 *
 * @param u the service that should be started.
 */
int service_start(Unit *u);

#endif
