#include <stdlib.h>
#include <stdio.h>
#include "da.h"
#include "die.h"

struct game_die{
	DA *faces;
	int seed;
};

extern GDIE *newGDIE(void){
	GDIE *dice = (GDIE*)malloc(sizeof(GDIE));
	dice->faces = newDA();
	dice->seed = 0;
}

extern void setGDIEfree(GDIE *dice, void (*f)(void *)){
	setDAfree(dice->faces, f); 
}

extern void setGDIEdisplay(GDIE *dice, (*d)(void *, FILE *)){
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

extern int setGDIEseed(GDIE *dice, int seed);

extern void *rollGDIE(GDIE *dice);

extern void printGDIE(GDIE *dice, FILE *where);

extern void freeGDIE(void *dice){
	GDIE *d = (GDIE *)dice;
	freeDA(d->faces);
	free(dice);
}

