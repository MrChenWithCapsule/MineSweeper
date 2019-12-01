#include <curses.h>
#include <pthread.h>
#include <stdlib.h>
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
char image4[][8] = {"You Win ", "You Lose"};

int expand(chtype **broad, int y, int x);

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
        chtype **broad = make_buffer(broad_row + 2, broad_col + 2);
        srand(time(NULL));
        int mine_n = (broad_col == 9 ? 10 : broad_col == 16 ? 40 : 99);
        for (int i = 0; i < broad_row + 2; ++i)
            for (int j = 0; j < broad_col + 2; ++j)
                broad[i][j] = 0;
        for (int j = 0; j < broad_col + 2; ++j)
            broad[0][j] = broad[broad_row + 1][j] = '-';
        int mine_gen = mine_n;
        while (mine_gen != 0)
        {
            for (int i = 1; i < broad_row + 1; ++i)
            {
                broad[i][0] = broad[i][broad_col + 1] = '|';
                for (int j = 1; j < broad_col + 1 && mine_gen != 0; ++j)
                {
                    if (broad[i][j] != '*' &&
                        rand() % (broad_row * broad_col) < mine_n)
                    {
                        broad[i][j] = '*';
                        --mine_gen;
                    }
                }
            }
        }
        for (int i = 1; i < broad_row + 1; ++i)
            for (int j = 1; j < broad_col + 1; ++j)
            {
                if (broad[i][j] != '*')
                {
                    int count = 0;
                    if ((broad[i - 1][j - 1] & 255) == '*')
                        ++count;
                    if ((broad[i - 1][j] & 255) == '*')
                        ++count;
                    if ((broad[i - 1][j + 1] & 255) == '*')
                        ++count;
                    if ((broad[i][j - 1] & 255) == '*')
                        ++count;
                    if ((broad[i][j + 1] & 255) == '*')
                        ++count;
                    if ((broad[i + 1][j - 1] & 255) == '*')
                        ++count;
                    if ((broad[i + 1][j] & 255) == '*')
                        ++count;
                    if ((broad[i + 1][j + 1] & 255) == '*')
                        ++count;
                    broad[i][j] = (count == 0 ? ' ' : count + '0');
                }
                init_pair(1, COLOR_WHITE, COLOR_WHITE);
                broad[i][j] |= COLOR_PAIR(1);
            }
        int base_row = (scr_row - broad_row - 2) / 2;
        int base_col = (scr_col - broad_col - 2) / 2;
        int broad_id = tui_add_element(
            (Position){base_row, base_col},
            (Position){base_row + broad_row + 1, base_col + broad_col + 1},
            broad);
        int status = 0;
        int square_left = broad_row * broad_col - mine_n;
        MEVENT event;
        int ch;
        while (status == 0)
        {
            ch = getch();
            ch = getch();
            if (ch != KEY_MOUSE)
                continue;
            getmouse(&event);
            if (event.y <= base_row || event.y >= base_row + broad_row + 1 ||
                event.x <= base_col || event.x >= base_col + broad_col + 1 ||
                (broad[event.y - base_row][event.x - base_col] & (~255)) == 0)
                continue;
            tui_lock();
            broad[event.y - base_row][event.x - base_col] &= 255;
            if (broad[event.y - base_row][event.x - base_col] == '*')
                status = 1;
            else
                square_left -=
                    expand(broad, event.y - base_row, event.x - base_col);
            tui_unlock();
            if (square_left == 0)
                status = 2;
        }
        chtype **message = make_buffer(1, 8);
        for (int j = 0; j < 8; ++j)
            message[0][j] = (status == 1 ? image4[1][j] : image4[0][j]);
        int message_id = tui_add_element(
            (Position){scr_row / 2, (scr_col - 8) / 2},
            (Position){scr_row / 2, (scr_col - 8) / 2 + 8}, message);
        getch();
        tui_delete_element(message_id);
        tui_delete_element(broad_id);
    }
}

int expand(chtype **broad, int y, int x)
{
    int count = 1;
    broad[y][x] &= 255;
    if (broad[y][x] != ' ')
        return 1;
    chtype ch;
    ch = broad[y - 1][x] & 255;
    if (((ch >= '1' && ch <= '9') || ch == ' ') && ch != broad[y - 1][x])
        count += expand(broad, y - 1, x);
    ch = broad[y][x - 1] & 255;
    if (((ch >= '1' && ch <= '9') || ch == ' ') && ch != broad[y][x - 1])
        count += expand(broad, y, x - 1);
    ch = broad[y + 1][x] & 255;
    if (((ch >= '1' && ch <= '9') || ch == ' ') && ch != broad[y + 1][x])
        count += expand(broad, y + 1, x);
    ch = broad[y][x + 1] & 255;
    if (((ch >= '1' && ch <= '9') || ch == ' ') && ch != broad[y][x + 1])
        count += expand(broad, y, x + 1);
    return count;
}
