#include "mboard.h"
#include "boardutil.h"

#define MBOARD_WINDOW_WIDTH (BOARD_SIZE * BOARD_WINDOW_WIDTH + 6)
#define MBOARD_WINDOW_HEIGHT (BOARD_SIZE * BOARD_WINDOW_HEIGHT + 8)

static int Mboard_InputCursor(Mboard *board);

static int Mboard_InputCursor(Mboard *board){
	if(!board->choose_cursor) return 0;

	int ch;

	ch = getch();

	switch(ch){
		case '\n':
			board->choose_cursor = 0;
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

Mboard * Mboard_Create(int pos_x, int pos_y){
	Mboard *board = (Mboard *) malloc(sizeof(Mboard));

	for(int i = 0; i < MBOARD_SIZE; i++){
		for(int j = 0; j < MBOARD_SIZE; j++){
			board->board[i][j] = Sboard_Create(
					1 + pos_x + i * (BOARD_WINDOW_WIDTH + 3),
					1 + pos_y + j * (BOARD_WINDOW_HEIGHT + 3)
					);
		}
	}

	board->win = newwin(
			MBOARD_WINDOW_HEIGHT,
			MBOARD_WINDOW_WIDTH,
			pos_y,
			pos_x
			);

	board->choose_cursor = 1;
	board->has_drawn_border = 0;

	board->cursor.x = board->cursor.y;
	board->choose_cursor = 1;
	board->player = 1;

	return board;
}

int Mboard_Input(Mboard *board){
	int input = 0;
	int new_cursor_x, new_cursor_y;

	if(board->choose_cursor)
		return Mboard_InputCursor(board);

	input = Sboard_Input(board->board[board->cursor.x][board->cursor.y], &board->player);

	if(input == INPUT_PUTPIECE){
		new_cursor_x = board->board[board->cursor.x][board->cursor.y]->cursor.x;
		new_cursor_y = board->board[board->cursor.x][board->cursor.y]->cursor.y;

		Mboard_Draw(board);

		board->cursor.x = new_cursor_x;
		board->cursor.y = new_cursor_y;
	}

	return input;
}

void Mboard_Draw(Mboard *board){
	int mul_x, mul_y;

	mul_x = (MBOARD_WINDOW_WIDTH / MBOARD_SIZE + 1);
	mul_y = (MBOARD_WINDOW_HEIGHT / MBOARD_SIZE + 1);

	if(!board->has_drawn_border){
		BoardUtil_Draw(board->win, MBOARD_WINDOW_WIDTH, MBOARD_WINDOW_HEIGHT, BOARD_SIZE);
		board->has_drawn_border = 1;

		wrefresh(board->win);

		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				Sboard_Draw(board->board[i][j]);
			}
		}
	}

	
	if(board->choose_cursor){
		wmove(
				board->win,
				board->cursor.y * mul_y + mul_y / 2,
				board->cursor.x * mul_x + mul_x / 2
			 );

		wrefresh(board->win);
	}
	else{
		Sboard_Draw(board->board[board->cursor.x][board->cursor.y]);
	}

}

void Mboard_Destroy(Mboard *board){
	for(int i = 0; i < MBOARD_SIZE; i++){
		for(int j = 0; j < MBOARD_SIZE; j++){
			Sboard_Destroy(board->board[i][j]);
		}
	}

	free(board);
}
