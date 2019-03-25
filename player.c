#include <stdlib.h>
#include <stdio.h>

struct player{
	char *name;
	int wins;
	int losses;
}

extern *PLAYR newPLAYR(char *n){
	PLAYR *p = (PLAYR*) malloc(sizeof(PLAYR));
	p->name = n;
	p->wins = 0;
	p->losses = 0;
}

extern getPLAYRwins(PLAYR *p){
	return p->wins;
}

extern getPLAYRlosses(PLAYR *p){
	return p->losses;
}

extern setPLAYRwins(PLAYR *p, int w){
	p->wins = w;
}

extern setPLAYRlosses(PLAYR *p, int l){
	p->losses = l;
}

extern void freePLAYR(PLAYR *p){
	if(p->name) free(p->name);
	free(p);
}
