#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <sys/types.h>
#include <sys/wait.h>

#ifndef HAVE_EVENTLOOP_IMPLEMENTATION
typedef struct EventLoop
{
} EventLoop;
#endif

typedef void (*event_child_handler_t)(pid_t pid, void *userdata);

EventLoop *event_loop_create_main_loop();
int event_loop_run(EventLoop *loop);
void event_loop_quit();
int event_loop_add_child(pid_t pid, event_child_handler_t handler, void *userdata);

#endif
