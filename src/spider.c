#include "scheduler.h"
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <stdio.h>

int strTime()
{
    time_t lt;
    struct tm *tm;
    char buf[64] = {0};

    lt = time(NULL);
    tm = localtime(&lt);
    strftime(buf, sizeof(buf), "%F %T", tm);
    printf("%s", buf);
}

int cb1(void* data)
{
    static i = 0;
    if (i < 50)
    {
        i++;
        strTime();
        fprintf(stdout, "-----111\n");
        fflush(stdout);
        return 1; // just return a non-zero value.
    }
    else
    {
        return 0; // will delete this scheduler.
    }
}

int cb2(void* data)
{
    static i = 0;
    if (i < 20)
    {
        i++;
        strTime();
        fprintf(stdout, "-----222222\n");
        fflush(stdout);
        return 1000;
    }
    else
    {
        return 0; // will delete this scheduler.
    }
}

int cb3(void* data)
{
    static i = 0;
    if (i < 5)
    {
        i++;
        strTime();
        fprintf(stdout, "-----33333333\n");
        fflush(stdout);
        return 10000;
    }
    else
    {
        return 0; // will delete this scheduler.
    }
}

//struct scheduler {
//	int flag;              /*!< Use return value from callback to reschedule */
//	int reschedule;        /*!< When to reschedule */
//	int id;                /*!< ID number of event */
//	struct timeval when;   /*!< Absolute time event should take place */
//	spd_scheduler_cb callback;
//	void *data;
//	SPD_LIST_ENTRY(scheduler)list;
//};
//
//    struct scheduler sched_1 = {
//        0, 5, 1, {0, 0}, cb1, NULL, {NULL}
//    }, sched_2 = {
//        1, 5, 1, {0, 0}, cb2, NULL, {NULL}
//    }, sched_3 = {
//        0, 5, 1, {0, 0}, cb3, NULL, {NULL}
//    };

void* func(void* data)
{
    struct scheduler_context *sc;
    int ms;

    sc = spd_sched_context_create();

    spd_sched_add_flag(sc, 1000, cb1,NULL, 0);
    spd_sched_add_flag(sc, 5000, cb2,NULL, 1);
    spd_sched_add_flag(sc, 1000, cb3,NULL, 1);

    while(-1 != (ms = spd_sched_wait(sc)))
    {
        poll(NULL, 0, ms);
        spd_sched_runall(sc);
    }
    spd_sche_context_destroy(sc);
    printf("thread exit!\n");
}

int main()
{
    pthread_t thread;

    pthread_create(&thread, NULL, func, NULL);

    pthread_join(thread, NULL);
}
