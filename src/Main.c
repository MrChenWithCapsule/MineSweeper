#ifdef USE_PDCURSES
#include <pdcurses.h>
#else
#include <curses.h>
#endif
#include <pthread.h>

#include "Game.h"
#include "Tui.h"

int main()
{
    pthread_t tui_thrd;
    pthread_create(&tui_thrd, NULL, &tui_init, NULL);
    game_start();

    while (!game_at_exit())
    {
        game_user_input();
    }

    tui_end();
    pthread_join(tui_thrd, NULL);
    return 0;
}
