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

#include "config.h"
#include "eventloop.h"
#include "unitmanager.h"
#include "target.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
    printf("\n\ninit 0.1 - Starting system...\n\n");

    EventLoop *loop = event_loop_create_main_loop();

    UnitManager *unitman = unitmanager_init();
    if (!unitman) {
        fprintf(stderr, "init: failed to initialize unit manager.\n");
        return EXIT_FAILURE;
    }

    unitmanager_loadunit(unitman, SYSTEM_TARGETS_PATH DEFAULT_BOOT_TARGET);
    unitmanager_start_all(unitmanager_build_dependencies_list(unitman));

    return event_loop_run(loop);
}
