#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "da.h"
#include "die.h"
#include "player.h"
#include "boggle.h"

struct boggle{
	GRID *guts;
	PLAYR *player1;		//always human
	PLAYR *player2;		//human or ai(lul) 
	DA *solve_list;
	DA *dice;		//list of 16 standard Boggle dice
	int seed;
};

static void populateDice(BOGG *b);
static void setDicePlacement(BOGG *b);
static void printDiceFace(void *, FILE *where);

/*******************************************************************************************************/
/* static void readPlayerLog(---); ---> this should be done via player.c i think. pass in log location */
/*******************************************************************************************************/

extern BOGG *newBOGG(int s, int r, int c){ 
	BOGG *b = (BOGG*) malloc(sizeof(BOGG));	
	b->seed = s;
	srand(s);
	b->guts = newGRID(showGDIE, NULL, r, c);
	b->player1 = NULL;
	b->player2 = NULL;
	b->solve_list = newDA();
	setDAfree(b->solve_list, free);	
	
	b->dice = newDA();
	setDAfree(b->dice, freeGDIE);
	populateDice(b);

	setDicePlacement(b);


	

	return b;
}

extern char getBOGGchar(BOGG *b, int r, int c){    	//make it so the characters in the GRID are unalterable
	char *ptr = (char*)getGRIDcell(b->guts, r, c);
	if(ptr)
		return ptr[0];
	else {
		printf("ptr was NULL - getBOGGchar");
		return 'F';
	}
}

static void setDicePlacement(BOGG *b){
	int size = getGRIDrows(b->guts) * getGRIDcols(b->guts);
	for (int i = 0; i < size; i++){
		printf("hihi\n");
		GDIE *cancer = (GDIE*)removeDA(b->dice, rand() % (size-i));
		rollGDIE(cancer);
		insertGRIDseq(b->guts, cancer);
	}
}

static void populateDice(BOGG *b){
	char *A[6] = { "a", "a" , "e" , "e" , "g" , "n" };
	char *B[6] = { "a", "b" , "b" , "j" , "o" , "o" };
	char *C[6] = { "a", "c" , "h" , "o" , "p" , "s" };
	char *D[6] = { "a", "f" , "f" , "k" , "p" , "s" };
	char *E[6] = { "a", "o" , "o" , "t" , "t" , "w" };
	char *F[6] = { "c", "i" , "m" , "o" , "t" , "u" };
	char *G[6] = { "d", "e" , "i" , "l" , "r" , "x" };
	char *H[6] = { "d", "e" , "l" , "r" , "v" , "y" };
	char *I[6] = { "d", "i" , "s" , "t" , "t" , "y" };
	char *J[6] = { "e", "e" , "g" , "h" , "n" , "w" };
	char *K[6] = { "e", "e" , "i" , "n" , "s" , "u" };
	char *L[6] = { "e", "h" , "r" , "t" , "v" , "w" };
	char *M[6] = { "e", "i" , "o" , "s" , "s" , "t" };
	char *N[6] = { "e", "l" , "r" , "t" , "t" , "y" };
	char *O[6] = { "h", "i" , "m" , "n" , "u" , "Qu" };
	char *P[6] = { "h", "l" , "n" , "n" , "r" , "z" };

	for (int i = 0; i < 16; i++){
		GDIE *die = newGDIE();
		setGDIEdisplay(die, printDiceFace);
		setGDIEseed(die, b->seed);
		insertDAback(b->dice, die);
	}
	setGDIEfaces(getDA(b->dice, 0), (void**)A, 6);
	setGDIEfaces(getDA(b->dice, 1), (void**)B, 6);
	setGDIEfaces(getDA(b->dice, 2), (void**)C, 6);
	setGDIEfaces(getDA(b->dice, 3), (void**)D, 6);
	setGDIEfaces(getDA(b->dice, 4), (void**)E, 6);
	setGDIEfaces(getDA(b->dice, 5), (void**)F, 6);
	setGDIEfaces(getDA(b->dice, 6),(void**)G, 6);
	setGDIEfaces(getDA(b->dice, 7), (void**)H, 6);
	setGDIEfaces(getDA(b->dice, 8), (void**)I, 6);
	setGDIEfaces(getDA(b->dice, 9), (void**)J, 6);
	setGDIEfaces(getDA(b->dice, 10), (void**)K, 6);
	setGDIEfaces(getDA(b->dice, 11), (void**)L, 6);
	setGDIEfaces(getDA(b->dice, 12), (void**)M, 6);
	setGDIEfaces(getDA(b->dice, 13), (void**)N, 6);
	setGDIEfaces(getDA(b->dice, 14),(void**)O, 6);
	setGDIEfaces(getDA(b->dice, 15), (void**)P, 6);

}

extern void displayBOGG(BOGG *b, FILE *where){
	printGRID(b->guts, where);
}

static void printDiceFace(void *c, FILE *where){
	char *a = (char*)c;
	fprintf(where, "%c ", a[0]);
}


