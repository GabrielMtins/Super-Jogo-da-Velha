#ifndef SBOARD_H
#define SBOARD_H

#define BOARD_SIZE 3
#define BOARD_WINDOW_WIDTH 8
#define BOARD_WINDOW_HEIGHT 5

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

enum GAMESTATES{
	STATE_ONGOING = 0,
	STATE_P1WIN = 1,
	STATE_P2WIN = 2,
	STATE_DRAW
};

enum INPUTSTATES{
	INPUT_NONE = 0,
	INPUT_QUIT = 1,
	INPUT_PUTPIECE = 2,
};

typedef struct{
	WINDOW *win;

	int board[BOARD_SIZE][BOARD_SIZE];
	int has_drawn_border;
	
	struct{
		int x, y;
	} cursor;
} Sboard;

Sboard *Sboard_Create(int pos_x, int pos_y);

void Sboard_Reset(Sboard *board);

/* return one of the enumerations under GAMESTATES */
int Sboard_GetGameState(Sboard *board);

/* returns 0 if the input is to quit the program */
int Sboard_Input(Sboard *board, int *player);

void Sboard_Draw(Sboard *board);

void Sboard_Destroy(Sboard *board);

#endif
