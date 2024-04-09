#ifndef BOARDUTIL_H
#define BOARDUTIL_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

/* use wrefresh after this */
void BoardUtil_Draw(WINDOW *win, int board_window_width, int board_window_height, int board_size);

#endif
