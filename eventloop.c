#include "eventloop.h"

#include <stdlib.h>

EventLoop *event_loop_new(EventLoop *loop)
{
    return malloc(sizeof(EventLoop));
}


int event_loop_run(EventLoop *loop)
{
    return EXIT_SUCCESS;
}
