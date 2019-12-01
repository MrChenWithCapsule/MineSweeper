#include <curses.h>
#include <pthread.h>
#include <unistd.h>

#include "Buffer.h"
#include "CursesWrapper.h"
#include "Game.h"
#include "Sleep.h"
#include "Tui.h"
typedef struct Element
{
    Position up_l;
    Position low_r;
    chtype **buf;
} Element;

// Height: 12, Width: 48
char image1[][48] = {"             ___  ___ _                         ",
                     "             |  \\/  |(_)                        ",
                     "             | .  . | _  _ __    ___            ",
                     "             | |\\/| || || '_ \\  / _ \\           ",
                     " _____       | |  | || || | | ||  __/           ",
                     "/  ___|      \\_|  |_/|_||_| |_| \\___|           ",
                     "\\ `--. __      __  ___   ___  _ __    ___  _ __ ",
                     " `--. \\ \\ /\\ / / / _ \\ / _ \\| '_ \\  / _ \\| '__|",
                     "/\\__/ / \\ V  V / |  __/|  __/| |_) ||  __/| |   ",
                     "\\____/   \\_/\\_/   \\___| \\___|| .__/  \\___||_|   ",
                     "                             | |                ",
                     "                             |_|                "};
char image2[19] = "PRESS HERE TO START";

void game_run()
{
    int scr_row;
    int scr_col;
    // Initialize
    {
        getmaxyx(stdscr, scr_row, scr_col);
        if (scr_row < 14 || scr_col < 50)
        {
            // Abort
        }
        while (!tui_ready())
            usleep(100);
    }

    // Startscreen
    {
        Element start_image = {
            (Position){(scr_row - 14) / 2, (scr_col - 48) / 2},
            (Position){(scr_row - 14) / 2 + 11, (scr_col - 48) / 2 + 47},
            make_buffer(12, 48)};
        for (int i = 0; i < 12; ++i)
            for (int j = 0; j < 48; ++j)
                start_image.buf[i][j] = image1[i][j];
        Element start_button = {
            (Position){start_image.low_r.row + 2, (scr_col - 19) / 2},
            (Position){start_image.low_r.row + 2, (scr_col - 19) / 2 + 18},
            make_buffer(1, 19)};
        tui_lock();
        int image_id = tui_add_element(start_image.up_l, start_image.low_r,
                                       start_image.buf);
        int button_id = tui_add_element(start_button.up_l, start_button.low_r,
                                        start_button.buf);
        tui_unlock();
        MEVENT event;
        int ch;
        while (1)
        {
            ch = getch();
            if (ch != KEY_MOUSE && getmouse(&event) == OK &&
                (event.bstate & BUTTON1_CLICKED) &&
                tui_find_element((Position){event.y, event.x}) == button_id)
                break;
        }
        tui_lock();
        tui_delete_element(image_id);
        tui_delete_element(button_id);
        tui_unlock();
    }

    // Choose mode
    {}

    // Game
    {
    }
}
