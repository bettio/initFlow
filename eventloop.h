#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

typedef struct EventLoop
{
} EventLoop;

EventLoop *event_loop_new();
int event_loop_run(EventLoop *loop);

#endif
