#include "boardutil.h"

void BoardUtil_Draw(WINDOW *win, int board_window_width, int board_window_height, int board_size){
	int mul_x, mul_y;

	mul_x = (board_window_width / board_size + 1);
	mul_y = (board_window_height / board_size + 1);

	for(int i = 0; i < board_window_width; i++){
		for(int j = mul_y - 1; j < board_window_height - 1; j+= mul_y){
			wmove(win, j, i);
			waddch(win, '-');
		}
	}

	for(int j = 0; j < board_window_height; j++){
		for(int i = mul_x - 1; i < board_window_width - 1; i += mul_x){
			wmove(win, j, i);

			waddch(win, '|');
		}
	}

	for(int j = mul_y; j < board_window_height; j += mul_y){
		for(int i = mul_x; i < board_window_width - 1; i += mul_x){
			wmove(win, j - 1, i - 1);

			waddch(win, '+');
		}
	}
}
