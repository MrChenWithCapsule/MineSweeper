#include <curses.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "CursesWrapper.h"
#include "Sleep.h"
#include "Tui.h"

typedef struct ElementList
{
    int id;
    Position up_l;
    Position low_r;
    chtype **buf;
    struct ElementList *next;
} ElementList;

ElementList *begin, *current;
int curr_id;
pthread_mutex_t display_lock = PTHREAD_MUTEX_INITIALIZER;

int status = 0;
pthread_mutex_t status_lock = PTHREAD_MUTEX_INITIALIZER;

void tui_routine();

void *tui_run(void *_)
{
    initscr();
    cbreak();
    // noecho();
    keypad(stdscr, true);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    pthread_mutex_lock(&display_lock);
    pthread_mutex_lock(&status_lock);
    status = 1;
    begin = current = malloc(sizeof(ElementList));
    curr_id = 0;
    pthread_mutex_unlock(&status_lock);
    pthread_mutex_lock(&display_lock);
    tui_routine();
    pthread_mutex_destroy(&display_lock);
    pthread_mutex_destroy(&status_lock);
    return NULL;
}

void tui_end()
{
    pthread_mutex_lock(&display_lock);
    pthread_mutex_lock(&status_lock);
    status = 0;
    for (ElementList *p = begin; p != NULL;)
    {
        ElementList *p2 = p->next;
        free(p);
        p = p2;
    }
    begin = current = NULL;
    pthread_mutex_unlock(&status_lock);
    pthread_mutex_lock(&display_lock);
    endwin();
}

int tui_add_element(Position upper_left, Position lower_right, chtype **buffer)
{
    ElementList *tmp = malloc(sizeof(ElementList));
    tmp->up_l = upper_left;
    tmp->low_r = lower_right;
    tmp->buf = buffer;
    tmp->next = NULL;
    pthread_mutex_lock(&display_lock);
    int id = curr_id++;
    tmp->id = id;
    current = current->next = tmp;
    pthread_mutex_unlock(&display_lock);
    return id;
}

void tui_delete_element(int element_id)
{
    pthread_mutex_lock(&display_lock);
    for (ElementList *p = begin; p->next != NULL; p = p->next)
    {
        if (p->next->id == element_id)
        {
            ElementList *tmp = p->next;
            p->next = tmp->next;
            free(tmp);
        }
    }
    pthread_mutex_unlock(&display_lock);
}

int tui_find_element(Position pos)
{
    int id = -1;
    pthread_mutex_lock(&display_lock);
    for (ElementList *p = begin->next; p != NULL; p = p->next)
        if (p->up_l.row <= pos.row && pos.row <= p->low_r.row &&
            p->up_l.column <= pos.column && pos.column <= p->low_r.column)
            id = p->id;
    pthread_mutex_unlock(&display_lock);
    return id;
}

void tui_lock()
{
    pthread_mutex_lock(&display_lock);
}

void tui_unlock()
{
    pthread_mutex_unlock(&display_lock);
}

int tui_ready()
{
    pthread_mutex_lock(&status_lock);
    int ready = status;
    pthread_mutex_unlock(&status_lock);
    return ready;
}

void tui_routine()
{
    while (1)
    {
        clear();
        int cury;
        int curx;
        getyx(stdscr, cury, curx);
        pthread_mutex_lock(&display_lock);
        pthread_mutex_lock(&status_lock);
        if (status == 0)
            break;
        for (ElementList *p = begin->next; p != NULL; p = p->next)
        {
            for (int i = 0; i <= p->low_r.row - p->up_l.row; ++i)
                for (int j = 0; j <= p->low_r.column - p->up_l.column; ++j)
                    mvaddch(p->up_l.row + i, p->up_l.column + j, p->buf[i][j]);
        }
        pthread_mutex_unlock(&status_lock);
        pthread_mutex_unlock(&display_lock);
        move(cury, curx);
        refresh();
        usleep(100);
    }
}
