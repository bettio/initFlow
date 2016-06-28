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

#ifndef _EVENTLOOPSTRUCTS_H_
#define _EVENTLOOPSTRUCTS_H_

#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>

#define EVENT_TYPE_NONE 0
#define EVENT_TYPE_CHILD 1

#define HAVE_EVENTLOOP_IMPLEMENTATION
typedef struct EventLoop
{
    void *listeners[256];
    int listeners_count;
    int keep_running;
} EventLoop;

typedef struct EventListener
{
    void *handler;
    void *userdata;
    uint8_t type;
} EventListener;

typedef struct ChildListener
{
    EventListener parent_instance;
    pid_t pid;
} ChildListener;

#endif
