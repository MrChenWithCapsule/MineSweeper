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
// Height: 1, Width: 19
char image2[19] = "PRESS HERE TO START";
// Height: 1x4, Width: 24
char image3[][24] = {"Choose mode:            ", "Easy (8x8, 10 Mines)    ",
                     "Medium (16x16, 40 Mines)", "Expert (30x16, 99 Mines)"};

void game_run()
{
    int scr_row;
    int scr_col;
    // Initialize
    {
        while (!tui_ready())
            usleep(100);
        getmaxyx(stdscr, scr_row, scr_col);
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
        for (int j = 0; j < 19; ++j)
            start_button.buf[0][j] = image2[j];
        int image_id = tui_add_element(start_image.up_l, start_image.low_r,
                                       start_image.buf);
        int button_id = tui_add_element(start_button.up_l, start_button.low_r,
                                        start_button.buf);
        MEVENT event;
        int ch;
        while (1)
        {
            ch = getch();
            if (ch == KEY_MOUSE && getmouse(&event) == OK &&
                tui_find_element((Position){event.y, event.x}) == button_id)
                break;
        }
        // tui_lock();
        tui_delete_element(image_id);
        tui_delete_element(button_id);
        // tui_unlock();
        delete_buffer(start_image.buf, 12);
        delete_buffer(start_button.buf, 1);
    }

    // Choose mode
    int broad_row, broad_col;
    {
        int base_y = (scr_row - 4) / 2;
        int base_x = (scr_col - 24) / 2;
        Element buttons[4] = {
            {(Position){base_y, base_x}, (Position){base_y, base_x + 23},
             make_buffer(1, 24)},
            {(Position){base_y + 1, base_x},
             (Position){base_y + 1, base_x + 23}, make_buffer(1, 24)},
            {(Position){base_y + 2, base_x},
             (Position){base_y + 2, base_x + 23}, make_buffer(1, 24)},
            {(Position){base_y + 3, base_x},
             (Position){base_y + 3, base_x + 23}, make_buffer(1, 24)}};
        int buttons_id[4];
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 24; ++j)
                buttons[i].buf[0][j] = image3[i][j];
            buttons_id[i] = tui_add_element(buttons[i].up_l, buttons[i].low_r,
                                            buttons[i].buf);
        }
        MEVENT event;
        int ch;
        while (1)
        {
            ch = getch();
            if (ch != KEY_MOUSE || getmouse(&event) != OK)
                continue;
            int id = tui_find_element((Position){event.y, event.x});
            if (id == buttons_id[1])
            {
                broad_row = 9;
                broad_col = 9;
                break;
            }
            else if (id == buttons_id[2])
            {
                broad_row = 16;
                broad_col = 16;
                break;
            }
            else if (id == buttons_id[3])
            {
                broad_row = 16;
                broad_col = 30;
                break;
            }
        }
        for (int i = 0; i < 4; ++i)
        {
            tui_delete_element(buttons_id[i]);
            delete_buffer(buttons[i].buf,
                          buttons[i].low_r.row - buttons[i].up_l.row);
        }
    }

    // Game
    {
    }
}
