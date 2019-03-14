#include <ncurses.h>
#include "da.h"
#include "dictionary.h"
#include <stdio.h>


WINDOW *create_newwin(int height, int width, int starty, int startx, char fill);
void destroy_win(WINDOW *local_win);

int main()
{
	FILE *doc = fopen("words_alpha.txt", "r");
	DICT *d = newDictionary(doc);
	printf("%d\n", sizeDICT(d));
	DA *grid = newDA();
	int sizew = 20;
	int sizeh = 10;
	for(int i = 0; i < sizeh; i++){
		DA *a = newDA();
		insertDAback(grid, a);
	}
	WINDOW *my_win;
	int startx, starty, width, height;
	int ch;

	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	height = 3;
	width = 5;
	starty = 2;	/* Calculating for a center placement */
	startx = 3;	/* of the window		*/
	printw("Press F1 to exit");
	refresh();
	int x = 0;
	int y = 0;
	char k = 'a';
	for(int i = 0; i < sizeh; i++){
		for(int j = 0; j < sizew; j++){
			insertDAback(getDA(grid, i), my_win = create_newwin(height, width, starty+y, startx+x, k++));
			x += 4;
		}
		k = 'a';
		x = 0;
		y += 2;
	}
	/*my_win = create_newwin(height, width, starty, startx);
	for(int i = height-1; i > 1; i-=2){
		wmove(my_win, i, 1);
		whline(my_win, ACS_HLINE, width-2);
	}
	wmove(my_win, height/2, width/2);
	waddch(my_win, 'R');
	*/
	wrefresh(my_win);

	while((ch = getch()) != KEY_F(1))
	{
		/*
		switch(ch)
		{	case KEY_LEFT:
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
				break;	
		}
		*/
	}
		
	endwin();			/* End curses mode		  */
	return 0;
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
