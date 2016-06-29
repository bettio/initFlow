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

#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define EVENT_TYPE_NONE 0
#define EVENT_TYPE_CHILD 1

struct _EventLoop
{
    void *listeners[256];
    int listeners_count;
    int keep_running;
};

typedef struct EventListener
{
    void *userdata;
    uint8_t type;
} EventListener;

typedef struct ChildListener
{
    EventListener parent_instance;

    event_child_handler_t handler;
    pid_t pid;
} ChildListener;


static EventLoop *main_event_loop;

EventLoop *event_loop_create_main_loop()
{
    main_event_loop = malloc(sizeof(EventLoop));
    main_event_loop->listeners_count = 0;
    return main_event_loop;
}

int event_loop_run(EventLoop *loop)
{
    main_event_loop->keep_running = 1;

    while (main_event_loop->keep_running) {
        int status = 0;
        pid_t pid = waitpid(-1, &status, 0);
        if (pid > 0) {
            for (int i = 0; i < main_event_loop->listeners_count; i++) {
                EventListener *listener = (EventListener *) main_event_loop->listeners[i];
                if (listener->type == EVENT_TYPE_CHILD) {
                    ChildListener *child = (ChildListener *) listener;
                    if (child->pid == pid) {
                        event_child_handler_t handler = child->handler;
                        handler(pid, listener->userdata);
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

void event_loop_append_listener(void *listener)
{
    main_event_loop->listeners[main_event_loop->listeners_count] = listener;
    main_event_loop->listeners_count++;
}

void event_loop_quit()
{
    main_event_loop->keep_running = 0;
}

int event_loop_add_child(pid_t pid, event_child_handler_t handler, void *userdata)
{
    ChildListener *child_listener = malloc(sizeof(ChildListener));

    child_listener->parent_instance.type = EVENT_TYPE_CHILD;
    child_listener->parent_instance.userdata = userdata;
    child_listener->pid = pid;
    child_listener->handler = handler;

    event_loop_append_listener(child_listener);

    return 0;
}
