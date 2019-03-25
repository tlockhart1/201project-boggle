#include <stdlib.h>
#include <stdio.h>
#include "player.h"

struct player{
	char *name;
	int wins;
	int losses;
};

extern PLAYR *newPLAYR(char *n){
	PLAYR *p = (PLAYR*) malloc(sizeof(PLAYR));
	p->name = n;
	p->wins = 0;
	p->losses = 0;
	return p;
}

extern int getPLAYRwins(PLAYR *p){
	return p->wins;
}

extern int getPLAYRlosses(PLAYR *p){
	return p->losses;
}

extern void setPLAYRwins(PLAYR *p, int w){
	p->wins = w;
}

extern void setPLAYRlosses(PLAYR *p, int l){
	p->losses = l;
}

extern void freePLAYR(void *p){
	PLAYR *a = (PLAYR*)p;
	if(a->name) free(a->name);
	free(a);
}

extern void printPLAYR(void *player, FILE *where){
	PLAYR *p = (PLAYR*)player;
	fprintf(where, "Name: %s | Wins: %d | Losses: %d\n", p->name, p->wins, p->losses);
}
