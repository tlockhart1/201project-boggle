#include "rbt.h"
#include "dictionary.h"
#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct dictionary {
	RBT *tree;
};

static int compareWords(void *, void *);

extern DICT *newDictionary(){
	DICT *d = malloc(sizeof(DICT));
	d->tree = newRBT(compareWords);
	//if(doc) loadDICT(d, doc);
	return d;
}

extern int insertDICTword(DICT *d, void *data){
	if(insertRBT(d->tree, data)) return 1;
	return 0;
}

extern int deleteDICTword(DICT *d, void *data){
	return deleteRBT(d->tree, data);
}

extern int sizeDICT(DICT *d){
	return sizeRBT(d->tree);
}

extern void *getDICTword(DICT *d, void *data){
	return findRBT(d->tree, data);
}

static int compareWords(void *a, void *b){
	return strcmp(a, b);
}

extern void loadDICT(DICT *d, FILE *doc){
	char *string = NULL; 
	char ch;

	ch = fgetc(doc);
	if(ch == EOF){
		printf("Empty file??? wtf...\n");
	       	return;
	}
	while(ch != EOF){
		if(isspace(ch)) string = getToken(doc);
		else {
			ungetc(ch, doc);
			string = getToken(doc);
		}
		if(string){
		       insertDICTword(d, string);
		}
		string = NULL;
		ch = fgetc(doc); // I DON'T FUCKING GET WHY I NEED TWO HERE, SHOULD BE JUST ONE RETURN CARRIAGE INBETWEEN WORDS
		ch = fgetc(doc);
	}
	
}

extern void loadDICTprefixes(DICT *d, FILE *doc){
	char *string = NULL; 
	char ch;

	ch = fgetc(doc);
	if(ch == EOF){
		printf("Empty file??? wtf...\n");
	       	return;
	}
	while(ch != EOF){
		if(isspace(ch)) string = getPrefix(doc);
		else {
			ungetc(ch, doc);
			string = getPrefix(doc);
		}
		if(string){
		       insertDICTword(d, string);
		}
		string = NULL;
		ch = fgetc(doc); // I DON'T FUCKING GET WHY I NEED TWO HERE, SHOULD BE JUST ONE RETURN CARRIAGE INBETWEEN WORDS
		ch = fgetc(doc);
	}
}
