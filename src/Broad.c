#include <stdlib.h>

#include "Broad.h"

Broad broad_create(int row, int column)
{
    Broad br;
    br.row = row;
    br.column = column;
    br.data = (int **)calloc(row, sizeof(int *));
    for (int i = 0; i < row; ++i)
        br.data[i] = (int *)calloc(column, sizeof(int));
    return br;
}

void broad_delete(Broad broad)
{
    for (int i = 0; i < broad.row; ++i)
        free(broad.data[i]);
    free(broad.data);
}
