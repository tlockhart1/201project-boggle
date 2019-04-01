#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "da.h"

/** partially ripped from lusth's scanner file **/
static char *reallocateString(char *, size_t);
static void skipWhiteSpace(FILE *);

extern char *getToken(FILE *fp){
	char ch;
	int size = 50;
	char *string = malloc(size);
	int index = 0;
	int unwanted = 0;
	skipWhiteSpace(fp);
	ch = fgetc(fp);
	if(ch == EOF) return NULL;
	while(!isspace(ch)){
		if(ch == EOF) break;
		if(islower(ch) == 0 || isalpha(ch) == 0)
			unwanted = 1;
		if(index > size - 2){
			size *= 2;
			string = reallocateString(string, size);
		}
		string[index++] = ch;
		ch = fgetc(fp);
	}

	ungetc(ch, fp);

	if (index > 0)              //there is something in the buffer
        	clearerr(fp);           //so force the read to be good

	string[index] = '\0';
	if(index == 0 || unwanted == 1){
		free(string);
		return NULL;
	}
	return string;
}

extern char *getPrefix(FILE *fp){
	char ch;
	int size = 50;
	char *string = malloc(size);
	int index = 0;
	int unwanted = 0;
	skipWhiteSpace(fp);
	ch = fgetc(fp);
	if(ch == EOF) return NULL;
	while(!isspace(ch)){
		if(ch == EOF) break;
		if(islower(ch) == 0 || isalpha(ch) == 0)
			unwanted = 1;
		if(index > size - 2){
			size *= 2;
			string = reallocateString(string, size);
		}
		if(index < 3)
			string[index++] = ch;
		ch = fgetc(fp);
	}

	ungetc(ch, fp);

	if (index > 0)              //there is something in the buffer
        	clearerr(fp);           //so force the read to be good

	string[index] = '\0';
	if(index == 0 || unwanted == 1){
		free(string);
		return NULL;
	}
	return string;
}

extern char *getRawToken(FILE *fp){
	char ch;
	int size = 50;
	char *string = malloc(size);
	int index = 0;
	skipWhiteSpace(fp);
	ch = fgetc(fp);
	if(ch == EOF) return NULL;
	while(!isspace(ch)){
		if(ch == EOF) break;
		if(index > size - 2){
			size *= 2;
			string = reallocateString(string, size);
		}
		string[index++] = ch;
		ch = fgetc(fp);
	}

	ungetc(ch, fp);

	if (index > 0)              //there is something in the buffer
        	clearerr(fp);           //so force the read to be good

	string[index] = '\0';
	if(index == 0){
		free(string);
		return NULL;
	}
	return string;
}

extern void parseNStore(char *line, DA *words){
	char *goddamnit = strtok(line, " ");
	if(goddamnit)
		insertDAback(words, goddamnit);
	else return;

	while(1){
		char *store_dis = strtok(NULL, " ");
		if(store_dis)
			insertDAback(words, store_dis);
		else
			break;
	}
}

static char *reallocateString(char *string, size_t size){		
	char *newstring;

	newstring = realloc(string, size);

	return newstring;
}

/**this was taken directly from Lusth's scanner.c**/
static void
skipWhiteSpace(FILE *fp)
    {
    int ch;

    /* read chars until a non-whitespace character is encountered */

    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        continue;

    /* a non-space character got us out of the loop, so push it back */

    if (ch != EOF) ungetc(ch,fp);
    }

