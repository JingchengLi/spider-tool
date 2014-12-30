#include "scheduler.h"
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>


int cb1(void* data)
{
    perror("I am cb1\n");
    return 1;
}

int cb2(void* data)
{
    perror("I am cb2\n");
    return 1;
}

int cb3(void* data)
{
    perror("I am cb3\n");
    return 10;
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
    struct scheduler_context *sc = data;
    int ms;

    spd_sched_add_flag(sc, 5, cb1,NULL, 0);
    spd_sched_add_flag(sc, 5, cb2,NULL, 1);
    spd_sched_add_flag(sc, 1, cb3,NULL, 1);

    while(-1 != (ms = spd_sched_wait(sc)))
    {
        TEMP_FAILURE_RETRY(poll(NULL, 0, ms));
        spd_sched_runall(sc);
    }
}

int main()
{
    struct scheduler_context *sc;

    struct timeval tv = {60, 0};
    pthread_t thread;

    sc = spd_sched_context_create();

    pthread_create(&thread, NULL, func, sc);

    select(0,NULL,NULL,NULL,&tv);
    spd_sched_del(sc, 1);
    spd_sched_del(sc, 2);
    spd_sched_del(sc, 3);

    pthread_join(thread, NULL);
    spd_sche_context_destroy(sc);
}
