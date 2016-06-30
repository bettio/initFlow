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

#include "ptrlist.h"

#include <poll.h>
#include <sys/signalfd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define EVENT_TYPE_NONE 0
#define EVENT_TYPE_CHILD 1

struct _EventLoop
{
    PtrList *listeners;
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
    if (!main_event_loop) {
        return NULL;
    }

    main_event_loop->listeners = ptr_list_new();
    if (!main_event_loop->listeners) {
        free(main_event_loop);
        return NULL;
    }

    return main_event_loop;
}

int event_loop_run(EventLoop *loop)
{
    main_event_loop->keep_running = 1;

#ifdef SIMPLE_EVENT_LOOP
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
#else

    sigset_t mask;
    sigemptyset (&mask);
    sigaddset (&mask, SIGCHLD);
    sigprocmask (SIG_BLOCK, &mask, NULL);

    int sfd = signalfd(-1, &mask, SFD_CLOEXEC);

    struct pollfd fds[1];
    fds[0].fd = sfd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    while (main_event_loop->keep_running) {
        int status;
        poll(fds, 1, -1);

        pid_t pid = waitpid(-1, &status, 0);
        if (pid > 0) {
            int listeners_count = ptr_list_count(main_event_loop->listeners);
            for (int i = 0; i < listeners_count; i++) {
                EventListener *listener = (EventListener *) ptr_list_at(main_event_loop->listeners, i);
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
#endif

    return EXIT_SUCCESS;
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

    if (ptr_list_append(main_event_loop->listeners, child_listener) < 0) {
        free(child_listener);
        return -1;
    }

    return 0;
}
