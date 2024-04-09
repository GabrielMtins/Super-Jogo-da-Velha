#include "sboard.h"
#include "boardutil.h"

static void Sboard_PutPiece(Sboard *board, int player);

static void Sboard_PutPiece(Sboard *board, int player){
	board->board[board->cursor.x][board->cursor.y] = player;
}

Sboard *Sboard_Create(int pos_x, int pos_y){
	Sboard *board = (Sboard *) malloc(sizeof(Sboard));

	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			board->board[i][j] = 0;
		}
	}

	board->cursor.x = 0;
	board->cursor.y = 0;
	board->has_drawn_border = 0;

	board->win = newwin(BOARD_WINDOW_HEIGHT, BOARD_WINDOW_WIDTH, pos_y, pos_x);

	return board;
}

void Sboard_Reset(Sboard *board){
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			board->board[i][j] = 0;
		}
	}

	board->cursor.x = 0;
	board->cursor.y = 0;
}

int Sboard_GetGameState(Sboard *board){
	int same;

	for(int i = 0; i < BOARD_SIZE; i++){
		same = 1;

		for(int j = 0; j < BOARD_SIZE - 1; j++){
			if(board->board[i][j] != board->board[i][j + 1]) same = 0;
		}

		if(same) return board->board[i][0];
	}

	for(int j = 0; j < BOARD_SIZE; j++){
		same = 1;

		for(int i = 0; i < BOARD_SIZE - 1; i++){
			if(board->board[i][j] != board->board[i + 1][j]) same = 0;
		}

		if(same) return board->board[0][j];
	}

	same = 1;

	for(int i = 0; i < BOARD_SIZE - 1; i++){
		if(board->board[i][i] != board->board[i + 1][i + 1]) same = 0;
	}

	if(same) return board->board[0][0];

	same = 1;

	for(int i = 0; i < BOARD_SIZE - 1; i++){
		if(board->board[BOARD_SIZE - 1 - i][i] != board->board[BOARD_SIZE - i - 2][i + 1]) same = 0;
	}

	if(same) return board->board[BOARD_SIZE - 1][0];

	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			if(board->board[i][j] == 0) return STATE_ONGOING;
		}
	}

	return STATE_DRAW;
}

int Sboard_Input(Sboard *board, int *player){
	int ch;

	ch = getch();

	switch(ch){
		case '\n':
			if(Sboard_GetGameState(board) == STATE_ONGOING){
				Sboard_PutPiece(board, *player);
				(*player)++; 
				if(*player > 2) *player = 1;
			}
			return INPUT_PUTPIECE;
			break;

		case 'q':
			return INPUT_QUIT;
			break;

		case 'a':
		case KEY_LEFT:
			board->cursor.x--;
			break;

		case 'd':
		case KEY_RIGHT:
			board->cursor.x++;
			break;

		case 'w':
		case KEY_UP:
			board->cursor.y--;
			break;

		case 's':
		case KEY_DOWN:
			board->cursor.y++;
			break;
	}

	if(board->cursor.x < 0) board->cursor.x = 0;
	if(board->cursor.y < 0) board->cursor.y = 0;
	if(board->cursor.x >= BOARD_SIZE) board->cursor.x = BOARD_SIZE - 1;
	if(board->cursor.y >= BOARD_SIZE) board->cursor.y = BOARD_SIZE - 1;

	return INPUT_NONE;
}

void Sboard_Draw(Sboard *board){
	int x, y;
	int mul_x, mul_y;

	mul_x = (BOARD_WINDOW_WIDTH / BOARD_SIZE + 1);
	mul_y = (BOARD_WINDOW_HEIGHT / BOARD_SIZE + 1);

	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			x = i * mul_x;
			y = j * mul_y;

			wmove(board->win, y, x);

			if(board->board[i][j] == 0)
				waddch(board->win, ' ');
			else if(board->board[i][j] == 1)
				waddch(board->win, 'X');
			else if(board->board[i][j] == 2)
				waddch(board->win, 'O');
		}
	}

	if(!board->has_drawn_border){
		BoardUtil_Draw(board->win, BOARD_WINDOW_WIDTH, BOARD_WINDOW_HEIGHT, BOARD_SIZE);
		board->has_drawn_border = 1;
	}

	wmove(board->win, board->cursor.y * mul_y, board->cursor.x * mul_x);

	wrefresh(board->win);
}

void Sboard_Destroy(Sboard *board){
	free(board);
}
