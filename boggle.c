#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "da.h"

struct boggle{
	GRID *guts;
	PLAYR *player1;		//always human
	PLAYR *player2;		//human or ai(lul) 
	DA *solve_list;

}

extern BOG *newBOGG(void (*d)(void *, FILE *), void (*f)(void *), int seed, int r, int c){
	BOG *b = (BOGG*) malloc(sizeof(BOGG));	
	b->guts = newGRID(d, f, r, c);
	b->player1 = NULL;
	b->player2 = NULL;
	b->solve_list = newDA();
	setDAfree(b->solve_list, free);	

	return b;
}
