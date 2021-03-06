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
#include "rbt.h"
#include "trie.h"
#include <string.h>
#include <ctype.h>

typedef struct cell {
	int visit_flag;
	int column_pos;
	int row_pos;
	void *value;
} CELL;

struct boggle{
	GRID *guts;
	DA *solve_list;
	DA *dice;		//list of 16 standard Boggle dice
	DICT *diction;
	RBT *search_list;
	int seed;
};

static void populateDice(BOGG *b);
static void setDicePlacement(BOGG *b);
static void printDiceFace(void *, FILE *where);
static void printCELL(void *cell, FILE *where);
static int comparator(void *A , void *B);

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
	b->search_list = newRBT(comparator);
	srand(s);
	b->guts = newGRID(printCELL, NULL, r, c);
	b->solve_list = newDA();
	setDAfree(b->solve_list, free);	
	b->diction = newDictionary();
	b->dice = newDA();
	setDAfree(b->dice, freeGDIE);
	populateDice(b);
	setDicePlacement(b);

	return b;
}

extern void loadBOGGdict(BOGG *b){
	FILE *file = fopen("words.txt", "r");
	loadDICT(b->diction, file);
	fclose(file);
}

extern char *accessBOGGsolved(BOGG *b, int index){
	if((sizeDA(b->solve_list) == 0) && (sizeDA(b->solve_list)-1 < index))
		return NULL;
	return getDA(b->solve_list, index);

}

extern int sizeBOGGsolved(BOGG *b){
	return sizeDA(b->solve_list);
}

extern void solveBOGG(BOGG *b, int r, int c, char *word, int index){ //dunno gonna implement RBT next and see if it finds shit
	CELL *current = getGRIDcell(b->guts, r, c);
	current->visit_flag = 1;
	word[index++] = tolower(((char*)(current->value))[0]);
	if(word[index-1] == 'q')
		word[index++] = 'u';
	word[index] = '\0';
	int check = 0;
	if(index >= 3){
		check = getDICTword(b->diction, word);
		if(check == 1){
			char *found = malloc(100);
			strcpy(found, word);
			if(freqRBT(b->search_list, found) == 0){
				insertRBT(b->search_list, found);
				insertDAback(b->solve_list, found);
				//printf("%s\n", word);
			}
		}

	}
	if(getDICTprefix(b->diction, word) == 1){
		for(int i = r - 1; (i <= r + 1) && (i < getGRIDrows(b->guts)); i++){
			for(int j = c - 1; j <= c + 1 && j < getGRIDcols(b->guts); j++){
				if(i >= 0 && j >= 0 && ((CELL*)(getGRIDcell(b->guts, i, j)))->visit_flag == 0)
					solveBOGG(b, i, j, word, index);
			}
		}
	}
	if(index > 1){
		if(word[index-1] == 'u' && word[index-2] == 'q')
			index--;
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

extern DA *getBOGGsolved(BOGG *b){
	return b->solve_list;
}

static void setDicePlacement(BOGG *b){
	int size = getGRIDrows(b->guts) * getGRIDcols(b->guts);
	DA *kill_me_haha = newDA();
	for (int i = 0; i < size; i++){
		if(sizeDA(b->dice) == 0){
			freeDA(b->dice);
			b->dice = kill_me_haha;
			kill_me_haha = newDA();
		}
		GDIE *cancer = (GDIE*)removeDA(b->dice, rand() % (sizeDA(b->dice)));
		insertDAback(kill_me_haha, cancer);
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
	fprintf(where,"%c,%d,%d ", a[0], c->row_pos, c->column_pos);
}

static void printDiceFace(void *c, FILE *where){
	char *a = (char*)c;
	fprintf(where, "%c ", a[0]);
}

static int comparator(void *A , void *B){
	char *a = (char*)A;
	char *b = (char*)B;
	return strcmp(a, b);
}

extern int scoreBOGGwords(BOGG *b, DA *w){
	RBT *dupes = newRBT(comparator);
	int score = 0;
	for(int i = 0; i < sizeDA(w); i++){
		if(findRBT(b->search_list, getDA(w, i))){
			if(freqRBT(dupes, getDA(w, i)) == 0)
				insertRBT(dupes, getDA(w, i));
			else{
				removeDA(w, i);
				i--;
			}
		}
		else{
		       removeDA(w, i);
		       i--;
		}
	}
	for(int i = 0; i < sizeDA(w); i++){
		int length = strlen(getDA(w, i));
		if(length == 3)
			score += 1;
		else if(length == 4)
			score += 2;
		else if(length == 5)
			score += 3;
		else if(length == 6)
			score += 4;
		else if(length == 7)
			score += 5;
		else 
			score += 6;
	}
	return score;
}

extern int getBOGGcols(BOGG *b){
	return getGRIDcols(b->guts);
}

extern int getBOGGrows(BOGG *b){
	return getGRIDrows(b->guts);
}


