#ifndef MINESWEEPER_BUFFER_H
#define MINESWEEPER_BUFFER_H

#include "CursesWrapper.h"

chtype **make_buffer(int row, int column);
void delete_buffer(int column);

#endif
