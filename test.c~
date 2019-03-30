#include <ncurses.h>
#include "da.h"
#include "dictionary.h"
#include "boggle.h"
#include "grid.h"
#include "fileio.h"
#include "time.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

WINDOW *create_newwin(int height, int width, int starty, int startx, char fill);
WINDOW *create_textbox(int height, int width, int starty, int startx);
int start_menu();
WINDOW *create_menu(int height, int width, int starty, int startx);
WINDOW *userWords(DA *words, int index, int lines, int cols);
void printDefMessage(char *, int lines, int width);
void getWords(DA *, int player);
void draw_board_win(BOGG *board, GRID *display, int rows, int cols, int startx, int starty, int height, int width);
void destroy_board_win(GRID *display, int rows, int cols);
void wordListTitle(char *, int, int);
void printScore(DA *, BOGG *, int, int);
PLAYR *getPlayerName(int);
int getUsrCols();
int getUsrRows();
int checkNoInteger(char *);

//char *wgetstr(WINDOW*);

void destroy_win(WINDOW *local_win);

int main()
{
	initscr();
	cbreak();
	noecho();

	int mode = start_menu(); //initial choices
	int rows = getUsrRows();
	int columns = getUsrCols();
	PLAYR *player1 = getPlayerName(1);
	PLAYR *player2;
	if(mode == 0)
		player2 = getPlayerName(2);

	BOGG *board = newBOGG(time(NULL), rows, columns);
	loadBOGGdict(board);
	char *s = malloc(6000);
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			solveBOGG(board, i, j, s, 0);
		}
	}

	int ch;
					
	keypad(stdscr, TRUE);

	int height = 3;
	int width = 5;
	int starty = LINES/4;	/* Calculating for a center placement */
	int startx = (COLS/2)-(17/2);	/* of the window		*/

	printw("Press F1 to exit");
	refresh();

	int usr_words_index = 0;
	int usr_words_index2 = 0;

	WINDOW *usr_words = NULL;
	WINDOW *usr_words2 = NULL;
	DA *wordsp1 = newDA();
	DA *wordsp2 = NULL;
	GRID *g = newGRID(NULL, NULL, rows, columns);
	draw_board_win(board, g, rows, columns, startx, starty, height, width);

	while((ch = getch()) != KEY_F(1))
	{
		
		switch(ch)
		{	
			case 'r':
				getWords(wordsp1, 1);
				destroy_board_win(g, rows, columns);
				g = newGRID(NULL, NULL, rows, columns);
				draw_board_win(board, g, rows, columns, startx, starty, height, width);
				if(mode == 0){
					wordsp2 = newDA();
					getWords(wordsp2, 2);
					printScore(wordsp2, board, LINES/4+12, COLS - COLS/4);
					usr_words2 = userWords(wordsp2, 0, LINES/4, COLS - COLS/4);
					wordListTitle("Player 2", (LINES/4)-1, COLS - COLS/4);
					//printScore(wordsp2, board, LINES/4+12, COLS - COLS/4);
				}
				printScore(wordsp1, board, LINES/4+12, COLS/4);
				usr_words = userWords(wordsp1, 0, LINES/4, COLS/4);
				wordListTitle("Player 1", (LINES/4)-1, COLS/4);
				//printScore(wordsp1, board, LINES/4+12, COLS/4);
				if(mode == 1){
					int difficulty = rand() % 25;
					DA *copy = newDA();
					wordsp2 = newDA();
					for(int i = 0; i < sizeBOGGsolved(board); i++){
						insertDAback(copy, accessBOGGsolved(board, i));
					}

					if(difficulty > sizeDA(copy)-1) difficulty = sizeDA(copy)-1;

					for(int i = 0; i < difficulty; i++){
						insertDAback(wordsp2, removeDA(copy, rand() % sizeDA(copy)));	
					}
					freeDA(copy);
					printScore(wordsp2, board, LINES/4+12, COLS - COLS/4);
					usr_words2 = userWords(wordsp2, 0, LINES/4, COLS - COLS/4);
					wordListTitle("Computer", (LINES/4)-1, COLS - COLS/4);
					//printScore(wordsp2, board, LINES/4+12, COLS - COLS/4);

				}
				destroy_board_win(g, rows, columns);
				g = newGRID(NULL, NULL, rows, columns);
				draw_board_win(board, g, rows, columns, startx, starty, height, width);
				break;
			case 'u':
				if(usr_words){
					if(usr_words_index < sizeDA(wordsp1)-1){
						destroy_win(usr_words);
						usr_words = userWords(wordsp1, ++usr_words_index, LINES/4, COLS/4);
					}
				}
				break;
			case 'j':
				if(usr_words){
					if(usr_words_index > 0){
						destroy_win(usr_words);
						usr_words = userWords(wordsp1, --usr_words_index, LINES/4, COLS/4);
					}
				}
				break;
			case 'i':
				if(usr_words2){
					if(usr_words_index2 < sizeDA(wordsp2)-1){
						destroy_win(usr_words2);
						usr_words2 = userWords(wordsp2, ++usr_words_index2, LINES/4, COLS - COLS/4);
					}
				}
				break;
			case 'k':
				if(usr_words2){
					if(usr_words_index2 > 0){
						destroy_win(usr_words2);
						usr_words2 = userWords(wordsp2, --usr_words_index2, LINES/4, COLS - COLS/4);
					}
				}

				break;
		}
		
	}


	destroy_board_win(g, rows, columns);
	endwin();			/* End curses mode		  */
	return 0;
}

void printScore(DA *words, BOGG *board, int lines, int width){
	move(lines, width);
	printw("SCORE: %d", scoreBOGGwords(board, words)); 
	refresh();
}

PLAYR *getPlayerName(int which){
	PLAYR *player;
	nocbreak();
	echo();
	if(which == 1)
		printDefMessage("Enter Player 1's name:", LINES-21, (COLS/2)-50);
	else 
		printDefMessage("Enter Player 2's name:", LINES-21, (COLS/2)-50);
	WINDOW *textbox = create_textbox(2, 100, LINES-20, COLS/2-50);
	char *c = malloc(1000);
	wmove(textbox, 0, 0);
	wgetstr(textbox, c); //clean preceding and following whitespace
	wrefresh(textbox);
	wclear(textbox);
	wrefresh(textbox);
	printDefMessage("                                                                                     ", LINES-21, (COLS/2)-50); //clear prev message - not perminate hopefully
	destroy_win(textbox);
	cbreak();
	noecho();
	player = newPLAYR(c);
	return player;
}

int getUsrCols(){
	nocbreak();
	echo();
	printDefMessage("Enter the number of columns for your board size:", LINES-21, (COLS/2)-50);
	WINDOW *textbox = create_textbox(2, 100, LINES-20, COLS/2-50);
	char *c = malloc(1000);
	wmove(textbox, 0, 0);
	wgetstr(textbox, c); //clean preceding and following whitespace
	while(checkNoInteger(c) == 0){
		wclear(textbox);
		destroy_win(textbox);

 		/***clear prev message - not perminate hopefully***/
		printDefMessage("                                                                                     ", LINES-21, (COLS/2)-50);

		textbox = create_textbox(2, 100, LINES-20, COLS/2-50);
		printDefMessage("Type DIGITS for the number of columns please:", LINES-21, (COLS/2)-50);
		wmove(textbox, 0, 0);
		wgetstr(textbox, c); //clean preceding and following whitespace

	}
	wrefresh(textbox);
	wclear(textbox);
	wrefresh(textbox);
	printDefMessage("                                                                                     ", LINES-21, (COLS/2)-50); //clear prev message - not perminate hopefully
	destroy_win(textbox);
	cbreak();
	noecho();
	int columns = atoi(c);
	free(c);
	return columns;
}

int checkNoInteger(char *input){
	for(int i = 0; input[i] != '\0'; i++){
		if(isdigit(input[i]) == 0)
			return 0;
	}
	return 1;
}

int getUsrRows(){
	nocbreak();
	echo();
	printDefMessage("Enter the number of rows for your board size:", LINES-21, (COLS/2)-50);
	WINDOW *textbox = create_textbox(2, 100, LINES-20, COLS/2-50);
	char *c = malloc(1000);
	wmove(textbox, 0, 0);
	wgetstr(textbox, c); //clean preceding and following whitespace
	while(checkNoInteger(c) == 0){
		wclear(textbox);
		destroy_win(textbox);

 		/***clear prev message - not perminate hopefully***/
		printDefMessage("                                                                                     ", LINES-21, (COLS/2)-50);

		textbox = create_textbox(2, 100, LINES-20, COLS/2-50);
		printDefMessage("Type DIGITS for the number of rows please:", LINES-21, (COLS/2)-50);
		wmove(textbox, 0, 0);
		wgetstr(textbox, c); //clean preceding and following whitespace

	}
	wrefresh(textbox);
	wclear(textbox);
	wrefresh(textbox);
	printDefMessage("                                                                                     ", LINES-21, (COLS/2)-50); //clear prev message - not perminate hopefully
	destroy_win(textbox);
	cbreak();
	noecho();
	int rows = atoi(c);
	free(c);
	return rows;
}

WINDOW *userWords(DA *words, int index, int lines, int cols){
	WINDOW *display = newwin(10, 15, lines, cols);
	int max;

	if(sizeDA(words)-1 < 10+index)
		max = sizeDA(words);
	else max = 10+index;

	for(int i = index; i < max; i++){
		wprintw(display, "%s\n", (char*)getDA(words, i));
	}
	wrefresh(display);
	refresh();
	return display;
}

void getWords(DA *words, int player){
	echo();
	nocbreak();
	if(player == 1)
		printDefMessage("Player 1, Enter all words seperated by a space:", LINES-21, (COLS/2)-50);
	else
		printDefMessage("Player 2, Enter all words seperated by a space:", LINES-21, (COLS/2)-50);
	WINDOW *textbox = create_textbox(2, 100, LINES-20, COLS/2-50);

	char *c = malloc(1000);

	wmove(textbox, 0, 0);
	wgetstr(textbox, c); //clean preceding and following whitespace
	//wrefresh(textbox);
	wclear(textbox);
	wrefresh(textbox);
	printDefMessage("                                                                                     ", LINES-21, (COLS/2)-50); //clear prev message - not perminate hopefully
	parseNStore(c, words);
	wclear(textbox);
	destroy_win(textbox);
	cbreak();
	noecho();
}

void draw_board_win(BOGG *board, GRID *display, int rows, int cols, int startx, int starty, int height, int width){
	int x = 0;	
	int y = 0;
	WINDOW *my_win;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			insertGRIDseq(display, my_win = create_newwin(height, width, starty+y, startx+x, getBOGGchar(board, i, j)));
			x += 4;
		}
		x = 0;
		y += 2;
	}
}

void destroy_board_win(GRID *display, int rows, int cols){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			destroy_win((WINDOW*)getGRIDcell(display, i, j));
		}
	}	
	freeGRID(display);
	display = NULL;
}

void printDefMessage(char *msg, int lines, int width){
	move(lines, width);
	printw("%s", msg); 
	refresh();
}

void wordListTitle(char *msg, int lines, int width){
	move(lines, width);
	printw("***-%s's (valid) Words-***", msg); 
	refresh();
}

int start_menu(){
	const char *choices [3] = {"Person vs Person Start", "Person vs Computer Start", "Player Logs"};
	WINDOW *menu = create_menu(10, 50, LINES/2-4, COLS/2-10);
	int choice;
	int highlight = 0;
	cbreak();
	keypad(menu, true);
	while(1){

		for(int i = 0; i < 3; i++){
			if(i == highlight)
				wattron(menu, A_REVERSE);
			mvwprintw(menu, i+1, 1, choices[i]); 
			wattroff(menu, A_REVERSE);
		}

		choice = wgetch(menu);

		switch(choice){
			case KEY_UP:
			highlight--;
			if(choice == -1)
				choice = 0;
			break;
			case KEY_DOWN:
			highlight++;
			if(choice == 3)
				choice = 2;
			break;
			default:
			break;
		}
		if(choice == 10)
			break;
	}
	wclear(menu);
	destroy_win(menu);
	return highlight;
}

WINDOW *create_menu(int height, int width, int starty, int startx){
	WINDOW *menu = newwin(height, width, starty, startx);
	box(menu, 0, 0);
	refresh();
	wrefresh(menu);
	return menu;
}

WINDOW *create_newwin(int height, int width, int starty, int startx, char fill)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	wborder(local_win, '|', '|', '-', '-', '-', '-', '-', '-'); //?? what for corner
	//box(local_win, '|', '-');
	wmove(local_win, height/2, width/2);
	waddch(local_win, fill);	
			/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

WINDOW *create_textbox(int height, int width, int starty, int startx){
	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '); //?? what for corner
	//box(local_win, 0, 0);
	//wmove(local_win, height/2, width/2);
	scrollok(local_win, TRUE);
	wrefresh(local_win);

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}
