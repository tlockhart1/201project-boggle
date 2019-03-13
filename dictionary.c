#include "rbt.h"
#include <stdio.h>
#include <stdlib.h>

struct dictionary {
	RBT *tree;
};

static compareWords(char *, char *);
static loadDICT(FILE *doc, RBT *d);

extern DICT *newDictionary(FILE *doc){
	DICT *d = malloc(sizeof(DICT));
	d->tree = newRBT(compareWords);
	return d;
}

static compareWords(char *a, char *b){
	return strcmp(a, b);
}
