/*
 * This file contains some utilities to help manage the display.
 */
#ifndef MINESWEEPER_TUI_H
#define MINESWEEPER_TUI_H

#include <curses.h>
typedef struct Position
{
    int row;
    int column;
} Position;

void *tui_run(void *);
void tui_end();

// Add an element in the window, return its id.
// A two-dimension array is provided to contain the glyphs of the element;
// it must be at least as big as the size of the element.
int tui_add_element(Position upper_left, Position lower_right, chtype **buffer);

// Delete an element, the caller must delete the buffer by itself.
void tui_delete_element(int element_id);

// Find the element at pos, return its id.
int tui_find_element(Position pos);

void tui_lock();
void tui_unlock();

#endif
