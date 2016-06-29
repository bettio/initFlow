/**
 * @file eventloop.h
 * @brief simple event loop.
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

#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <sys/types.h>
#include <sys/wait.h>

struct _EventLoop typedef EventLoop;

typedef void (*event_child_handler_t)(pid_t pid, void *userdata);

/**
 * @brief creates main event loop for this process.
 *
 * @details instantiates main event loop for this process, that will be globally available.
 * This function returns a pointer to the main loop.
 *
 * @return a pointer to the main event loop.
 */
EventLoop *event_loop_create_main_loop();

/**
 * @brief runs the even loop pointed by loop.
 *
 * @details runs the event loop and it exits when event_loop_quit() is called.
 *
 * @param loop the event loop that will be runned.
 * @return event loop exit status (e.g. EXIT_SUCCESS, EXIT_FAILURE or any user defined value).
 */
int event_loop_run(EventLoop *loop);

/**
 * @brief quits the event loop
 *
 * @details quits the event loop that is being runned.
 */
void event_loop_quit();

/**
 * @brief monitors a process pid for events.
 *
 * @details registers an event handler that will be called when a process exits.
 *
 * @param pid the process id that should be monitored.
 * @param handler the function pointer that should be called.
 * @param a pointer to some user defined data that will be passed to the handler when it gets called.
 *
 * returns 0 on success, otherwise -1 is returned.
 */
int event_loop_add_child(pid_t pid, event_child_handler_t handler, void *userdata);

#endif
