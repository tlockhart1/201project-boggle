#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "da.h"
#include "die.h"
#include "player.h"
#include "queue.h"
#include "stack.h"
#include "boggle.h"
#include "dictionary.h"

typedef struct cell {
	int visit_flag;
	int column_pos;
	int row_pos;
	void *value;
} CELL;

struct boggle{
	GRID *guts;
	PLAYR *player1;		//always human
	PLAYR *player2;		//human or ai(lul) 
	DA *solve_list;
	DA *dice;		//list of 16 standard Boggle dice
	DICT *diction;
	int seed;
};

static void populateDice(BOGG *b);
static void setDicePlacement(BOGG *b);
static void printDiceFace(void *, FILE *where);
static void printCELL(void *cell, FILE *where);
static void chooseNeighbor(STACK *s, BOGG *b, CELL *current);

/*******************************************************************************************************/
/* static void readPlayerLog(---); ---> this should be done via player.c i think. pass in log location */
/*******************************************************************************************************/

static CELL *newCELL(void *v, int r, int c){
	CELL *cll = (CELL*)malloc(sizeof(CELL));
	cll->column_pos = c;
	cll->row_pos = r;
	cll->visit_flag = 0;
	cll->value = v;
	return cll;
}

extern BOGG *newBOGG(int s, int r, int c){ 
	BOGG *b = (BOGG*) malloc(sizeof(BOGG));	
	b->seed = s;
	srand(s);
	b->guts = newGRID(printCELL, NULL, r, c);
	b->player1 = NULL;
	b->player2 = NULL;
	b->solve_list = newDA();
	setDAfree(b->solve_list, free);	
	FILE *file = fopen("words_alpha.txt", "r");
	b->diction = newDictionary(file);
	fclose(file);
	
	b->dice = newDA();
	setDAfree(b->dice, freeGDIE);
	populateDice(b);

	setDicePlacement(b);


	

	return b;
}
static void chooseNeighbor(STACK *s, BOGG *b, CELL *current){
	CELL *e;
	if(current->row_pos != 0){
		e = getGRIDcell(b->guts, current->row_pos - 1, current->column_pos);
		//if(e->visit_flag == 0)
		push(s, e);
	}
	if(current->column_pos != 0){
		e = getGRIDcell(b->guts, current->row_pos, current->column_pos - 1);
		//if(e->visit_flag == 0)
		push(s, e);
	}
	if(current->row_pos != getGRIDrows(b->guts)-1){
		e = getGRIDcell(b->guts, current->row_pos + 1, current->column_pos);
		//if(e->visit_flag == 0)
		push(s, e);
	}
	if(current->column_pos != getGRIDcols(b->guts)-1){
		e = getGRIDcell(b->guts, current->row_pos, current->column_pos + 1);
		//if(e->visit_flag == 0)
		push(s, e);
	}
	if((current->row_pos != 0) && (current->column_pos !=0)){
		e = getGRIDcell(b->guts, current->row_pos -1, current->column_pos - 1);
		//if(e->visit_flag == 0)
		push(s, e);
	}
	if((current->row_pos != getGRIDrows(b->guts)-1) && (current->column_pos != getGRIDcols(b->guts)-1)){
		e = getGRIDcell(b->guts, current->row_pos + 1, current->column_pos + 1);
		//if(e->visit_flag == 0)
		push(s, e);
	}
	if((current->row_pos != 0) && (current->column_pos != getGRIDcols(b->guts)-1)){
		e = getGRIDcell(b->guts, current->row_pos - 1, current->column_pos + 1);
		//if(e->visit_flag == 0)
		push(s, e);
	}
	if((current->row_pos != getGRIDrows(b->guts)-1) && (current->column_pos != 0)){
		e = getGRIDcell(b->guts, current->row_pos + 1, current->column_pos - 1);
		//if(e->visit_flag == 0)
		push(s, e);
	}
}

/*extern void solveBOGG(BOGG *b, int r, int c, int not_visited){
	char *word = (char*)malloc(getGRIDcols(b->guts)*getGRIDrows(b->guts));
	int total_visited = 0;
	STACK *im_dying = newSTACK();
	CELL *current = getGRIDcell(b->guts, r, c);
	push(im_dying, current);
	while(sizeSTACK(im_dying) > 0){
		current = pop(im_dying);
		if(current->visit_flag == not_visited){
			if(current->visit_flag == 1) current->visit_flag = 0;
			else current->visit_flag = 1;
			word[total_visited++] = ((char*)(current->value))[0];
			chooseNeighbor(im_dying, b, current);
			if(current->visit_flag == 1) current->visit_flag = 0;
			else current->visit_flag = 1;
		}
	}
	word[total_visited] = '\0';
	printf("%s\n", word);

}*/

extern void solveBOGG(BOGG *b, int r, int c, char *word, int index){ //dunno gonna implement RBT next and see if it finds shit
	//printf("hi\n");
	CELL *current = getGRIDcell(b->guts, r, c);
	current->visit_flag = 1;
	word[index++] = ((char*)(current->value))[0];
	word[index] = '\0';
	if((index >= 3) && (getDICTword(b->diction, word)))
		printf("%s\n", word);
	for(int i = r-1; i<=r+1 && i<getGRIDrows(b->guts); i++){
		for(int j = c-1; j<=c+1 && j<getGRIDcols(b->guts); j++){
			if(i >= 0 && j >= 0 && ((CELL*)(getGRIDcell(b->guts, i, j)))->visit_flag == 0)
				solveBOGG(b, i, j, word, index);
		}
	}
	index--;
	current->visit_flag = 0;
}

extern char getBOGGchar(BOGG *b, int r, int c){    	//make it so the characters in the GRID are unalterable
	CELL *pptr = (CELL*)getGRIDcell(b->guts, r, c);
	char *ptr = (char*)pptr->value;
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
		//printf("hihi\n");
		GDIE *cancer = (GDIE*)removeDA(b->dice, rand() % (size-i));
		rollGDIE(cancer);
		CELL *morecancer = newCELL(getGDIEtossed(cancer), getGRIDcr(b->guts), getGRIDcc(b->guts));
		insertGRIDseq(b->guts, morecancer);
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

static void printCELL(void *cell, FILE *where){
	CELL *c = (CELL*)cell;
	char *a = (char*)c->value;
	fprintf(where,"%c ", a[0]);
}

static void printDiceFace(void *c, FILE *where){
	char *a = (char*)c;
	fprintf(where, "%c ", a[0]);
}


