#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

static reallocateString(char *, int);

extern char *getToken(FILE *fp){
	char ch;
	int size = 50;
	char *string = malloc(size);
	int index = 0;

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
	string[index] = '\0';
	if(index == 0){
		free(string);
		return NULL;
	}
	return string;
}

static reallocateString(char *string, size_t size){		
	char *newstring;

	newstring = realloc(string, size);

	return newstring;
}

