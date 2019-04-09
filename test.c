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

int checkHeightBounds(int val);
int checkWidthBounds(int val);
WINDOW *create_newwin(int height, int width, int starty, int startx, char fill);
WINDOW *create_textbox(int height, int width, int starty, int startx);
int start_menu();
WINDOW *create_menu(int height, int width, int starty, int startx);
WINDOW *userWords(int score, char *name, DA *words, int height, int width, int index, int lines, int cols);
WINDOW *allWords(DA *words, int height, int width, int index, int lines, int cols);
WINDOW *scrollLog(DA *words, int height, int width, int index, int lines, int cols);
void printDefMessage(char *, int lines, int width);
void getWords(DA *, int player, int start);
WINDOW *draw_board_win(BOGG *board, GRID *display, int rows, int cols, int startx, int starty, int height, int width, int r, int c);
void destroy_board_win(WINDOW *title, GRID *display, int rows, int cols);
void wordListTitle(char *, int, int);
int calculateScore(BOGG *, DA *);
PLAYR *getPlayerName(int);
int getUsrCols();
int getUsrRows();
int setBoardxs();
int setBoardys();
int checkNoInteger(char *);
void displayLog();

//char *wgetstr(WINDOW*);

void destroy_win(WINDOW *local_win);

int main()
{
	initscr();
	cbreak();
	noecho();
	int mode = start_menu();
	while(mode == 2){
		displayLog();
		mode = start_menu();
	}
	if(mode == 3){
		endwin();
		exit(0);
	}
	int rows = getUsrRows();
	int columns = getUsrCols();

	while(rows * columns == 1){
		move(checkHeightBounds(LINES-22), checkWidthBounds((COLS/2)-50));
		printw("The product of the rows and columns cannot be equal to 1.");
		refresh();
		rows = getUsrRows();
		columns = getUsrCols();
		move(checkHeightBounds(LINES-22), checkWidthBounds((COLS/2)-50));
		printw("                                                         ");
		refresh();
	}
	int offset = 0;
	int p1score = 0;
	int p2score = 0;
	PLAYR *player1 = getPlayerName(0);
	PLAYR *player2 = NULL;
	if(mode == 0)
		player2 = getPlayerName(1);
	else
		player2 = getPlayerName(2);

	BOGG *board = newBOGG(time(NULL), rows, columns);
	loadBOGGdict(board);
	char *s = malloc(100);
	
	int ch;
					
	keypad(stdscr, TRUE);

	int height = 3;
	int width = 5;

	mvprintw(0,0,"Press F1 to exit");
	refresh();

	int usr_words_index = 0;
	int usr_words_index2 = 0;
	int all_words_index = 0;

	WINDOW *usr_words = NULL;
	WINDOW *usr_words2 = NULL;
	WINDOW *all_words = NULL;
	WINDOW *title = NULL;
	DA *wordsp1 = newDA();
	DA *wordsp2 = NULL;
	GRID *g = newGRID(NULL, NULL, rows, columns);
	int r = 0;
	int c = 0;
	int finished = 0;
	title = draw_board_win(board, g, rows, columns, setBoardxs(), setBoardys(), height, width, r, c);
	refresh();

	while((ch = getch()) != KEY_F(1))
	{
		
		switch(ch)
		{	
			case 'r':
				if(finished == 0){
					finished = 1;
					int start = setBoardys();
					getWords(wordsp1, 1, start);
					destroy_board_win(title, g, rows, columns);
					g = newGRID(NULL, NULL, rows, columns);
					title = draw_board_win(board, g, rows, columns, setBoardxs(), setBoardys(), height, width, r, c);
					refresh();
					for(int i = 0; i < rows; i++){
						for(int j = 0; j < columns; j++){
							solveBOGG(board, i, j, s, 0);
						}
					}
					if(mode == 0){
						wordsp2 = newDA();
						getWords(wordsp2, 2, start);
					}
					if(mode == 1){
						int difficulty = (rand() % 25) + 1;
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

					}
					p2score = calculateScore(board, wordsp2);
					if(mode == 1)
						usr_words2 = userWords(p2score, "Computer", wordsp2, 12, 22, 0, checkHeightBounds(LINES/4), checkWidthBounds(COLS - COLS/4));
					else if (mode == 0)
						usr_words2 = userWords(p2score, "Player 2", wordsp2, 12, 22, 0, checkHeightBounds(LINES/4), checkWidthBounds(COLS - COLS/4));
					p1score = calculateScore(board, wordsp1);

					usr_words = userWords(p1score, "Player 1", wordsp1, 12, 22, 0, LINES/4, checkWidthBounds(COLS/4-15));
					start = setBoardys();		
					all_words = allWords(getBOGGsolved(board), 17, 25, 0, checkHeightBounds(start+10), checkWidthBounds(COLS/2-12));

					destroy_board_win(title, g, rows, columns);
					g = newGRID(NULL, NULL, rows, columns);
					title = draw_board_win(board, g, rows, columns, setBoardxs(), setBoardys(), height, width, r, c);
					refresh();
				}
				break;
			case 'u':
				if(usr_words){
					if(usr_words_index < sizeDA(wordsp1)-1){
						destroy_win(usr_words);
						usr_words = userWords(p1score, "Player 1", wordsp1, 12, 22, ++usr_words_index, LINES/4, checkWidthBounds(COLS/4-15));
					}
				}
				break;
			case 'j':
				if(usr_words){
					if(usr_words_index > 0){
						destroy_win(usr_words);
						usr_words = userWords(p1score, "Player 1",wordsp1, 12, 22, --usr_words_index, LINES/4, checkWidthBounds(COLS/4-15));
					}
				}
				break;
			case 'i':
				if(usr_words2){
					if(usr_words_index2 < sizeDA(wordsp2)-1){
						destroy_win(usr_words2);
						if(mode == 1)
							usr_words2 = userWords(p2score, "Computer", wordsp2, 12, 22, ++usr_words_index2, LINES/4, checkWidthBounds(COLS - COLS/4));
						else if (mode == 0)
							usr_words2 = userWords(p2score, "Player 2", wordsp2, 12, 22, ++usr_words_index2, LINES/4, checkWidthBounds(COLS - COLS/4));
					}
				}
				break;
			case 'k':
				if(usr_words2){
					if(usr_words_index2 > 0){
						destroy_win(usr_words2);
						if(mode == 1)
							usr_words2 = userWords(p2score, "Computer", wordsp2, 12, 22, --usr_words_index2, LINES/4, checkWidthBounds(COLS - COLS/4));
						else if (mode == 0)
							usr_words2 = userWords(p2score, "Player 2", wordsp2, 12, 22, --usr_words_index2, LINES/4, checkWidthBounds(COLS - COLS/4));
					}
				}
				break;
			case 'o':
				if(all_words){
					if(all_words_index < sizeBOGGsolved(board)-1){
						destroy_win(all_words);
						int start = setBoardys();		
						all_words = allWords(getBOGGsolved(board), 17, 25, ++all_words_index, checkHeightBounds(start+10), checkWidthBounds(COLS/2-12));
					}
				}
				break;
			case 'l':
				if(all_words){
					if(all_words_index > 0){
						destroy_win(all_words);
						int start = setBoardys();		
						all_words = allWords(getBOGGsolved(board), 17, 25, --all_words_index, checkHeightBounds(start+10), checkWidthBounds(COLS/2-12));
					}
				}
				break;
			case 'a':
					if(c != 0)
						c--;
					destroy_board_win(title, g, rows, columns);
					g = newGRID(NULL, NULL, rows, columns);
					title = draw_board_win(board, g, rows, columns, setBoardxs(), setBoardys(), height, width, r, c);
				break;
			case 'd':
					if(columns < 4)
						offset = columns;
					else
						offset = 4;
					if(c != columns-offset)
						c++;
					destroy_board_win(title, g, rows, columns);
					g = newGRID(NULL, NULL, rows, columns);
					title = draw_board_win(board, g, rows, columns, setBoardxs(), setBoardys(), height, width, r, c);
				break;
			case 's':
					if(rows < 4)
						offset = rows;
					else offset = 4;
					if(r != rows-offset)
						r++;
					destroy_board_win(title, g, rows, columns);
					g = newGRID(NULL, NULL, rows, columns);
					title = draw_board_win(board, g, rows, columns, setBoardxs(), setBoardys(), height, width, r, c);
				break;
			case 'w':
					if(r != 0)
						r--;
					destroy_board_win(title, g, rows, columns);
					g = newGRID(NULL, NULL, rows, columns);
					title = draw_board_win(board, g, rows, columns, setBoardxs(), setBoardys(), height, width, r, c);
				break;

		}
		
	}
	updatePLAYRs(player1,player2, p1score, p2score);	
	writePLAYRlog(player1);
	writePLAYRlog(player2);
	destroy_board_win(title, g, rows, columns);
	endwin();			/* End curses mode		  */
	return 0;
}
int checkHeightBounds(int val){
	if(val <= 0)
		return 1;
	if(val >= LINES)
		return LINES-1;
	return val;
}
int checkWidthBounds(int val){
	if(val <= 0)
		return 1;
	if(val >= COLS)
		return COLS-1;
	return val;
}
int setBoardxs(){
	return checkWidthBounds((COLS/2)-(17/2));
}
int setBoardys(){
	return LINES/4;
}

void displayLog(){
	PLAYR *p = getPlayerName(3);
	noecho();
	cbreak();
	int ch;
	int index = 0;
	WINDOW *log = scrollLog(getPLAYRlog(p), 10, 30, index, LINES/2, checkWidthBounds(COLS/2-10));
	char *msg = malloc(100);
	strcpy(msg, getPLAYRname(p));
	strcat(msg, "'s Win/Loss Log:");
	printDefMessage(msg, checkHeightBounds(LINES/2-2), checkWidthBounds(COLS/2-10));
	while((ch = getch()) != 'b'){
		switch(ch){
			case 'u':
				if(index < sizeDA(getPLAYRlog(p))-1){
					destroy_win(log);
					log = scrollLog(getPLAYRlog(p), 10, 30, ++index, LINES/2, checkWidthBounds(COLS/2-10));
				}
			break;
			case 'j':
				if(index > 0){
					destroy_win(log);
					log = scrollLog(getPLAYRlog(p), 10, 30, --index, LINES/2, checkWidthBounds(COLS/2-10));
				}
			break;
		}
	}
	wclear(log);
	clear();
	refresh();
	free(msg);
	destroy_win(log);
}

int calculateScore(BOGG *board, DA *words){
	return scoreBOGGwords(board, words);
}

PLAYR *getPlayerName(int which){
	PLAYR *player;
	nocbreak();
	echo();
	if(which != 2){
		if(which == 0)
			printDefMessage("Enter Player 1's name:", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50));

		else if(which == 1)
			printDefMessage("Enter Player 2's name:", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50));
		else if(which == 3)
			printDefMessage("Enter Player's name:", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50));
		WINDOW *textbox = create_textbox(2, 100, checkHeightBounds(LINES-20), checkWidthBounds(COLS/2-50));
		char *c = malloc(1000);
		wmove(textbox, 0, 0);
		wgetstr(textbox, c); //clean preceding and following whitespace
		wrefresh(textbox);
		wclear(textbox);
		wrefresh(textbox);
		printDefMessage("                                                                                     ", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50)); //clear prev message - not perminate hopefully
		destroy_win(textbox);
		char *killme = malloc(1000);			
		strcpy(killme, c);
		strcat(killme, ".log");
		player = newPLAYR(c, killme);
	}
	else
		player = newPLAYR("Computer", "Computer.log");
	cbreak();
	noecho();
	return player;
}

int getUsrCols(){
	nocbreak();
	echo();
	printDefMessage("Enter the number of columns for your board size:", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50));
	WINDOW *textbox = create_textbox(2, 100, checkHeightBounds(LINES-20), checkWidthBounds(COLS/2-50));
	char *c = malloc(1000);
	wmove(textbox, 0, 0);
	wgetstr(textbox, c); //clean preceding and following whitespace
	while(checkNoInteger(c) == 0){
		wclear(textbox);
		destroy_win(textbox);

 		/***clear prev message - not perminate hopefully***/
		printDefMessage("                                                                                     ", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50));

		textbox = create_textbox(2, 100, checkHeightBounds(LINES-20), checkWidthBounds(COLS/2-50));
		printDefMessage("Type DIGITS for the number of columns please:", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50));
		wmove(textbox, 0, 0);
		wgetstr(textbox, c); //clean preceding and following whitespace

	}
	wrefresh(textbox);
	wclear(textbox);
	wrefresh(textbox);
	printDefMessage("                                                                                     ", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50)); //clear prev message - not perminate hopefully
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
	printDefMessage("Enter the number of rows for your board size:", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50));
	WINDOW *textbox = create_textbox(2, 100, checkHeightBounds(LINES-20), checkWidthBounds(COLS/2-50));
	char *c = malloc(1000);
	wmove(textbox, 0, 0);
	wgetstr(textbox, c); //clean preceding and following whitespace
	while(checkNoInteger(c) == 0){
		wclear(textbox);
		destroy_win(textbox);

 		/***clear prev message - not perminate hopefully***/
		printDefMessage("                                                                                     ", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50));

		textbox = create_textbox(2, 100, checkHeightBounds(LINES-20), checkWidthBounds(COLS/2-50));
		printDefMessage("Type DIGITS for the number of rows please:", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50));
		wmove(textbox, 0, 0);
		wgetstr(textbox, c); //clean preceding and following whitespace

	}
	wrefresh(textbox);
	wclear(textbox);
	wrefresh(textbox);
	printDefMessage("                                                                                     ", checkHeightBounds(LINES-21), checkWidthBounds((COLS/2)-50)); //clear prev message - not perminate hopefully
	destroy_win(textbox);
	cbreak();
	noecho();
	int rows = atoi(c);
	free(c);
	return rows;
}

WINDOW *scrollLog(DA *words, int height, int width, int index, int lines, int cols){
	WINDOW *display = newwin(height, width, lines, cols);
	int max;

	if(sizeDA(words)-1 < 10+index)
		max = sizeDA(words);
	else max = 10+index;

	for(int i = index; i < max; i++){
		PLAYR *p = (PLAYR *)getDA(words, i);
		wprintw(display, "VS %s: ", getPLAYRname(p));
		wprintw(display, "WON - %d, ", getPLAYRwins(p)); 
		wprintw(display, "LOST - %d\n", getPLAYRlosses(p));
	}
	wrefresh(display);
	refresh();
	return display;
}

WINDOW *userWords(int score, char *name, DA *words, int height, int width, int index, int lines, int cols){
	WINDOW *display = newwin(height, width, lines, cols);
	int max;
	wprintw(display, "**%s's words**", name);
	wmove(display, 1, 0);
	if(sizeDA(words)-1 == 0)
		max = sizeDA(words);
	else if(sizeDA(words) < (height+index-2))
		max = sizeDA(words);
	else max = height+index-2;
		
	for(int i = index; i < max; i++){
		wprintw(display, "%s\n", (char*)getDA(words, i));
	}
	wprintw(display, "Score: %d", score);
	wrefresh(display);
	refresh();
	return display;
}

WINDOW *allWords(DA *words, int height, int width, int index, int lines, int cols){
	if((LINES - lines) < height)
		height = (LINES - lines);
	WINDOW *display = newwin(height, width, lines, cols);
	int max;
	wprintw(display, "All Words:");
	wmove(display, 1, 0);
	if(sizeDA(words) < height+index-2)
		max = sizeDA(words);
	else max = height+index-2;
	for(int i = index; i < max; i++){
		wprintw(display, "%s\n", (char*)getDA(words, i));
	}
	wprintw(display, "%d Total Words", sizeDA(words));
	wrefresh(display);
	refresh();
	return display;
}

void getWords(DA *words, int player, int start){
	echo();
	nocbreak();
	if(player == 1)
		printDefMessage("Player 1, Enter all words seperated by a space:", checkHeightBounds(start+10), checkWidthBounds((COLS/2)-50));
	else
		printDefMessage("Player 2, Enter all words seperated by a space:", checkHeightBounds(start+10), checkWidthBounds((COLS/2)-50));
	WINDOW *textbox = create_textbox(2, 100, checkHeightBounds(start+11), checkWidthBounds(COLS/2-50));

	char *c = malloc(1000);

	wmove(textbox, 0, 0);
	wgetstr(textbox, c); //clean preceding and following whitespace
	wclear(textbox);
	wrefresh(textbox);
	printDefMessage("                                                                                     ", checkHeightBounds(start+10), checkWidthBounds((COLS/2)-50)); //clear prev message - not perminate hopefully
	parseNStore(c, words);
	wclear(textbox);
	destroy_win(textbox);
	cbreak();
	noecho();
}

WINDOW *draw_board_win(BOGG *board, GRID *display, int rows, int cols, int startx, int starty, int height, int width, int r, int c){
	int x = 0;	
	int y = 0;
	if(rows > 4)
		rows = 4;
	if(cols > 4)
		cols = 4;
	WINDOW *my_win;
	WINDOW *title; 
	title = newwin(1, 50, checkHeightBounds(starty-1),checkWidthBounds(startx));
	wmove(title, 0, 0);
	wprintw(title, "%d,%d", r, c);
	wrefresh(title);
	for(int i = 0; i < rows; i++){
		if((i+r) < getBOGGrows(board)){
			for(int j = 0; j < cols; j++){
				if((j+c) < getBOGGcols(board)){
					insertGRIDseq(display, my_win = create_newwin(height, width, starty+y, startx+x, getBOGGchar(board, i+r, j+c)));
					x += 4;
				}
				else break;
			}
		}
		else
			break;
		x = 0;
		y += 2;
	}
	return title;
}

void destroy_board_win(WINDOW *title, GRID *display, int rows, int cols){
	destroy_win(title);	
	if(rows > 4)
		rows = 4;
	if(cols > 4)
		cols = 4;
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
	const char *choices [4] = {"Person vs Person Start", "Person vs Computer Start", "Player Logs", "Exit"};
	WINDOW *menu = create_menu(10, 50, checkHeightBounds(LINES/2-4), checkWidthBounds(COLS/2-10));
	int choice;
	int highlight = 0;
	cbreak();
	keypad(menu, true);
	while(1){

		for(int i = 0; i < 4; i++){
			if(i == highlight)
				wattron(menu, A_REVERSE);
			mvwprintw(menu, i+1, 1, choices[i]); 
			wattroff(menu, A_REVERSE);
		}

		choice = wgetch(menu);

		switch(choice){
			case KEY_UP:
			highlight--;
			if(highlight == -1)
				highlight = 0;
			if(choice == -1)
				choice = 0;
			break;
			case KEY_DOWN:
			highlight++;
			if(highlight == 4)
				highlight = 3;
			if(choice == 4)
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
	wmove(local_win, height/2, width/2);
	waddch(local_win, fill);	
	wrefresh(local_win);	

	return local_win;
}

WINDOW *create_textbox(int height, int width, int starty, int startx){
	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '); //?? what for corner
	scrollok(local_win, TRUE);
	wrefresh(local_win);

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(local_win);
	wrefresh(local_win);
	delwin(local_win);
	refresh();
}

