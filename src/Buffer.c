#include <curses.h>
#include <stdlib.h>

#include "Buffer.h"

chtype **make_buffer(int row, int column)
{
    chtype **buf = calloc(row, sizeof(chtype *));
    for (int i = 0; i < row; ++i)
        buf[i] = calloc(column, sizeof(chtype));
    return buf;
}
void delete_buffer(chtype **buf, int row)
{
    for (int i = 0; i < row; ++i)
        free(buf[i]);
    free(buf);
}
