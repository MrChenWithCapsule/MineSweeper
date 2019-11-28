/*
 * This file contains the broad and utilities of it.
 */
#ifndef MINESWEEPER_BROAD_H
#define MINESWEEPER_BROAD_H

typedef struct BroadType
{
    int row;
    int column;
    int **data;
} Broad;

Broad broad_create(int row, int column);
Broad broad_delete(Broad broad);

#endif
