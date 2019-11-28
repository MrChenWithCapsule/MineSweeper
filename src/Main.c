#ifdef USE_PDCURSES
#include <pdcurses.h>
#else
#include <curses.h>
#endif
#include "Game.h"
#include "Tui.h"

int main()
{
    tui_init();
    game_start();

    while (!game_at_exit())
    {
        game_run();
    }

    tui_end();
}
