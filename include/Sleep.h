#ifndef MINESWEEPER_SLEEP_H
#define MINESWEEPER_SLEEP_H

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#ifndef usleep
inline void usleep(int time) { Sleep(time / 1000); }
#endif
#endif

#endif
