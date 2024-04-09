#ifndef MBOARD_H
#define MBOARD_H

#include "sboard.h"

#define MBOARD_SIZE 3

typedef struct{
	WINDOW *win;
	Sboard *board[MBOARD_SIZE][MBOARD_SIZE];
	int win_state;
	
	struct{
		int x, y;
	} cursor;

	int choose_cursor;
	int has_drawn_border;
	int player;
} Mboard;

Mboard * Mboard_Create(int pos_x, int pos_y);

/* returns 1 if it quits */
int Mboard_Input(Mboard *board);

void Mboard_Draw(Mboard *board);

void Mboard_Destroy(Mboard *board);

#endif
