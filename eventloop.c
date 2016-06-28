#include "eventloopstructs.h"
#include "eventloop.h"

#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>

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
                        event_child_handler_t handler = (event_child_handler_t) listener->handler;
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
    child_listener->pid = pid;
    child_listener->parent_instance.type = EVENT_TYPE_CHILD;
    child_listener->parent_instance.userdata = userdata;
    child_listener->parent_instance.handler = handler;

    event_loop_append_listener(child_listener);

    printf("added handler %p for %i\n", handler, pid);

    return 0;
}
