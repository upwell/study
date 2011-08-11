#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <event.h>

static void hello(int fd, short event, void *arg)
{
    struct event *p_tmevt = (struct event*) arg;
    printf("hello timer\n");

    /* add new timer */
    struct timeval tm;
    evutil_timerclear(&tm);
    tm.tv_sec = 1;
    evtimer_add(p_tmevt, &tm);
}

int main()
{
    struct event_base *evtbase;
    struct timeval *p_tm;
    struct event tmevt;

    evtbase = event_base_new();
    if(NULL == evtbase)
    {
        printf("cannot create event base\n");
        exit(-1);
    }

    p_tm = new struct timeval;

    evutil_timerclear(p_tm);
    p_tm->tv_sec = 1;

    evtimer_set(&tmevt, hello, &tmevt);
    event_base_set(evtbase, &tmevt);
    evtimer_add(&tmevt, p_tm);
    event_base_loop(evtbase, 0);

    return 0;
}
