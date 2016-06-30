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

#include "eventloop.h"
#include "unitmanager.h"
#include "target.h"

#include <stdio.h>
#include <stdlib.h>

#define BASE_PREFIX "/lib/"
#define SYSTEM_START_TARGET_PATH BASE_PREFIX "system/targets/system-start.target.requires/"

int main(int argc, char **argv, char **envp)
{
    printf("\n\ninit 0.1 - Starting system...\n\n");

    EventLoop *loop = event_loop_create_main_loop();

    UnitManager *unitman = unitmanager_init();
    if (!unitman) {
        fprintf(stderr, "init: failed to initialize unit manager.\n");
        return EXIT_FAILURE;
    }

    Target *target = target_load(unitman, SYSTEM_START_TARGET_PATH);
    if (!target) {
        fprintf(stderr, "init: failed to load target: %s.\n", SYSTEM_START_TARGET_PATH);
        return EXIT_FAILURE;
    }

    target_start_all(target);

    return event_loop_run(loop);
}
