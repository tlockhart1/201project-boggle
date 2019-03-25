#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "player.h"
#include <string.h>

void printString(void *, FILE *);

int main(){
	GRID *check = newGRID(printPLAYR, freePLAYR, 1, 3);
	char *aa = malloc(20);
	char *bb = malloc(20);
	char *cc = malloc(20);

	strcpy(aa, "William");
	strcpy(bb, "Billy Fucking Bob");
	strcpy(cc, "James Black");

	PLAYR *a = newPLAYR(aa);
	setPLAYRwins(a, 10);
	setPLAYRlosses(a, 5);
	PLAYR *b = newPLAYR(bb);
	setPLAYRwins(b, 8);
	setPLAYRlosses(b, 7);
	PLAYR *c = newPLAYR(cc);
	setPLAYRwins(c, 6);
	setPLAYRlosses(c, 9);

	fillGRIDrow(check, a, 0);
	fillGRIDrow(check, b, 0);
	fillGRIDrow(check, c, 0);

	printGRID(check, stdout);

	freeGRID(check);

	return 0;
}

void printString(void *item, FILE *where){
	char *str = (char*)item;
	fprintf(where, "%s", str);
}
