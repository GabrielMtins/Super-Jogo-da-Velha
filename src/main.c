#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#include "sboard.h"
#include "mboard.h"

enum MAINMENUOPTIONS{
	MAINMENU_JOGODAVELHA = 0,
	MAINMENU_SUPERJOGODAVELHA = 1,
	MAINMENU_SAIR = 2,
};

static int Main_Menu(void);

static void Main_JogoDaVelha(void);

static int Main_Menu(void){
	int option = 0, ch;

	clear();

	mvprintw(1, 3, "Jogo da Velha (Convencional)");
	mvprintw(3, 3, "Super Jogo da Velha");
	mvprintw(5, 3, "Sair");

	mvaddch(1 + option * 2, 1, '>');

	refresh();

	while(1){
		ch = getch();

		mvaddch(1 + option * 2, 1, ' ');

		switch(ch){
			case KEY_DOWN:
			case 's':
				option++;
				break;

			case KEY_UP:
			case 'w':
				option--;
				break;

			case '\n':
				return option;
		}

		option = option < 0 ? 0 : option;
		option = option >= MAINMENU_SAIR ? MAINMENU_SAIR : option;
		mvaddch(1 + option * 2, 1, '>');

		refresh();
	}
}

static void Main_JogoDaVelha(void){
	WINDOW *warning_window;
	Sboard *board;
	int player = 1;
	int game_state = STATE_ONGOING;
	int quit = 0;
	int input;

	warning_window = newwin(
			7,
			COLS,
			LINES - 7,
			0
			);

	board = Sboard_Create(
			(COLS - BOARD_WINDOW_WIDTH) / 2,
			(LINES - 7 - BOARD_WINDOW_HEIGHT) / 2
			);

	box(warning_window, 0, 0);
	mvwprintw(warning_window, 3, (COLS - strlen("Vez do Jogador 1")) / 2, "Vez do Jogador 1");

	clear();
	refresh();

	wrefresh(warning_window);
	Sboard_Draw(board);

	while(!quit){
		input = Sboard_Input(board, &player);

		if(input == INPUT_QUIT) quit = 1;
		if(input == INPUT_PUTPIECE && game_state != STATE_ONGOING){
			break;
		}

		if(player == 1){
			mvwprintw(warning_window, 3, (COLS - strlen("Vez do Jogador 1")) / 2, "Vez do Jogador 1");
		}
		else{
			mvwprintw(warning_window, 3, (COLS - strlen("Vez do Jogador 2")) / 2, "Vez do Jogador 2");
		}

		game_state = Sboard_GetGameState(board);

		if(game_state == STATE_P1WIN){
			mvwprintw(warning_window, 3, (COLS - strlen("Jogador 1 Venceu!")) / 2, "Jogador 1 Venceu!");
		}
		else if(game_state == STATE_P2WIN){
			mvwprintw(warning_window, 3, (COLS - strlen("Jogador 2 Venceu!")) / 2, "Jogador 2 Venceu!");
		}
		else if(game_state == STATE_DRAW){
			mvwprintw(warning_window, 3, (COLS - strlen("Empate")) / 2, "Empate");
		}

		wrefresh(warning_window);

		Sboard_Draw(board);
	}

	Sboard_Destroy(board);
}

int main(void){
	int quit = 0;

	initscr();
	keypad(stdscr, TRUE);
	refresh();
	noecho();

	while(!quit){
		switch(Main_Menu()){
			case MAINMENU_JOGODAVELHA:
				Main_JogoDaVelha();
				break;

			case MAINMENU_SAIR:
				quit = 1;
				break;
		}
	}
	

	endwin();
	return 0;
}
