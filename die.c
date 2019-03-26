#include <stdlib.h>
#include <stdio.h>
#include "da.h"
#include "die.h"

struct game_die{
	DA *faces;
	void *tossed;
	int seed;
};

extern GDIE *newGDIE(void){
	GDIE *dice = (GDIE*)malloc(sizeof(GDIE));
	dice->faces = newDA();
	dice->seed = 0;
	dice->tossed = NULL;
	srand(dice->seed);
	return dice;
}

extern void setGDIEfree(GDIE *dice, void (*f)(void *)){
	setDAfree(dice->faces, f); 
}

extern void setGDIEdisplay(GDIE *dice, void (*d)(void *, FILE *)){
	setDAdisplay(dice->faces, d);
}

extern void *getGDIEface(GDIE *dice, int index){
	return getDA(dice->faces, index);
}

extern void setGDIEfaces(GDIE *dice, void **items, int size){
	for (int i = 0; i < size; i++){
		insertDAback(dice->faces, items[i]);	
	}	
}

extern void *setGDIEface(GDIE *dice, int index, void *value){
	return setDA(dice->faces, index, value);	
}

extern int getGDIEsize(GDIE *dice){
	return sizeDA(dice->faces);
}

extern int setGDIEseed(GDIE *dice, int seed){
	int old = dice->seed;
	dice->seed = seed;
	srand(dice->seed);
	return old;
}

extern void *rollGDIE(GDIE *dice){
	return dice->tossed = getDA(dice->faces, rand() % sizeDA(dice->faces));
}

extern void printGDIE(GDIE *dice, FILE *where){
	displayDA(dice->faces, where);
}

extern void freeGDIE(void *dice){
	GDIE *d = (GDIE *)dice;
	freeDA(d->faces);
	free(dice);
}

