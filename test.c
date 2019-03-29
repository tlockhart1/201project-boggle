#include <ncurses.h>
#include "da.h"
#include "dictionary.h"
#include "boggle.h"
#include "grid.h"
#include "fileio.h"
#include "time.h"
#include <stdio.h>
#include <string.h>

WINDOW *create_newwin(int height, int width, int starty, int startx, char fill);
WINDOW *create_textbox(int height, int width, int starty, int startx);
int start_menu();
WINDOW *create_menu(int height, int width, int starty, int startx);
WINDOW *userWords(DA *words, int index, int lines, int cols);
void printDefMessage(char *, int lines, int width);
void getWords(DA *, int player);

//char *wgetstr(WINDOW*);

void destroy_win(WINDOW *local_win);

int main()
{
	WINDOW *my_win = NULL;
	int columns = 4;
	int rows = 4;

	initscr();
	cbreak();
	noecho();

	int mode = start_menu(); //initial choices

	BOGG *board = newBOGG(time(NULL), columns, rows);
	loadBOGGdict(board);
	char *s = malloc(6000);
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			solveBOGG(board, i, j, s, 0);
		}
	}

	int startx, starty, width, height;
	int ch;
					
	keypad(stdscr, TRUE);

	height = 3;
	width = 5;
	starty = LINES/4;	/* Calculating for a center placement */
	startx = (COLS/2)-(17/2);	/* of the window		*/
	printw("Press F1 to exit");
	refresh();
	int x = 0;
	int y = 0;
	GRID *g = newGRID(NULL, NULL, columns, rows);
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			insertGRIDseq(g, my_win = create_newwin(height, width, starty+y, startx+x, getBOGGchar(board, i, j)));
			x += 4;
		}
		x = 0;
		y += 2;
	}

	int usr_words_index = 0;
	int usr_words_index2 = 0;

	WINDOW *usr_words = NULL;
	WINDOW *usr_words2 = NULL;
	DA *wordsp1 = newDA();
	DA *wordsp2 = NULL;

	while((ch = getch()) != KEY_F(1))
	{
		
		switch(ch)
		{	/*case KEY_LEFT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,--startx);
				break;
			case KEY_RIGHT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,++startx);
				break;
			case KEY_UP:
				destroy_win(my_win);
				my_win = create_newwin(height, width, --starty,startx);
				break;
			case KEY_DOWN:
				destroy_win(my_win);
				my_win = create_newwin(height, width, ++starty,startx);
				break;	*/
			case 'r':
				getWords(wordsp1, 1);
				if(mode == 0){
					wordsp2 = newDA();
					getWords(wordsp2, 2);
					usr_words2 = userWords(wordsp2, 0, LINES/4, COLS - COLS/4);
				}
				usr_words = userWords(wordsp1, 0, LINES/4, COLS/4);
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
					usr_words2 = userWords(wordsp2, 0, LINES/4, COLS - COLS/4);
				}
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

	endwin();			/* End curses mode		  */
	return 0;
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
	wrefresh(textbox);
	wclear(textbox);
	wrefresh(textbox);
	printDefMessage("                                                              ", LINES-21, (COLS/2)-50); //clear prev message - not perminate hopefully
	parseNStore(c, words);
	
	cbreak();
	noecho();
}

void printDefMessage(char *msg, int lines, int width){
	move(lines, width);
	printw("%s", msg); 
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
