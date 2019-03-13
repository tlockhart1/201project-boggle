#include "rbt.h"
#include <stdio.h>
#include <stdlib.h>

struct dictionary {
	RBT *tree;
};

static compareWords(void *, void *);
static loadDICT(FILE *doc, RBT *d);

extern DICT *newDictionary(FILE *doc){
	DICT *d = malloc(sizeof(DICT));
	d->tree = newRBT(compareWords);
	return d;
}


