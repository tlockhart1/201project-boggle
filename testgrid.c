#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "player.h"
#include "die.h"
#include "boggle.h"
#include <string.h>

void printString(void *, FILE *);

int main(){
	GDIE *g = newGDIE();
	char *f[6] = { "a", "b", "c", "d", "e", "f" };
	setGDIEfaces(g, (void**)f, 6);
	for(int i = 0; i < 10; i++){
		char *roll = (char*)rollGDIE(g);
		printf("%c has been rolled.\n", roll[0]);
	}
	printf("\nSeed set from %d to %d\n\n", setGDIEseed(g, 10), 10);
	for(int i = 0; i < 10; i++){
		char *roll = (char*)rollGDIE(g);
		printf("%c has been rolled.\n", roll[0]);
	}

	freeGDIE(g);

	BOGG *killme = newBOGG(1012, 4, 4);
	displayBOGG(killme, stdout);
	char *word = malloc(10000);
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			solveBOGG(killme, i, j,word, 0);
		}
	}
	return 0;
}

void printString(void *item, FILE *where){
	char *str = (char*)item;
	fprintf(where, "%s", str);
}
