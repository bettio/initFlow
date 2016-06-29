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

#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <sys/types.h>
#include <sys/wait.h>

struct _EventLoop typedef EventLoop;

typedef void (*event_child_handler_t)(pid_t pid, void *userdata);

EventLoop *event_loop_create_main_loop();
int event_loop_run(EventLoop *loop);
void event_loop_quit();
int event_loop_add_child(pid_t pid, event_child_handler_t handler, void *userdata);

#endif
