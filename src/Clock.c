#include <curses.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "Broad.h"
#include "Buffer.h"
#include "Clock.h"
#include "Sleep.h"
#include "Tui.h"

int clock_started = 0;
pthread_mutex_t clock_status_lock = PTHREAD_MUTEX_INITIALIZER;

time_t clock_begin;

void *clock_start(void *args)
{
    Position *upper_left = args;
    clock_started = 1;
    chtype **buf = make_buffer(1, 10);
    char chrbuf[10];
    int clock_id = tui_add_element(
        *upper_left, (Position){upper_left->row, upper_left->column + 9}, buf);
    clock_begin = time(NULL);
    while (1)
    {
        pthread_mutex_lock(&clock_status_lock);
        int status_now = clock_started;
        pthread_mutex_unlock(&clock_status_lock);
        if (status_now == 0)
            break;
        time_t clock_now = time(NULL);
        tui_lock();
        sprintf(chrbuf, "Time: %d", (int)(difftime(clock_now, clock_begin)));
        for (int i = 0; i < 10; ++i)
        {
            buf[0][i] = chrbuf[i];
            buf[0][i] |= A_REVERSE;
        }
        tui_unlock();
        usleep(10000);
    }
    pthread_mutex_destroy(&clock_status_lock);
    tui_delete_element(clock_id);
    return NULL;
}
int clock_end()
{
    pthread_mutex_lock(&clock_status_lock);
    clock_started = 0;
    pthread_mutex_unlock(&clock_status_lock);
    return (int)(difftime(time(NULL), clock_begin));
}
