#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/** partially ripped from lusth's scanner file **/
static char *reallocateString(char *, size_t);

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

	if (index > 0)              //there is something in the buffer
        	clearerr(fp);           //so force the read to be good

	string[index] = '\0';
	if(index == 0){
		printf("wtf\n");
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

	ch = fgetc(fp);
	if(ch == EOF) return NULL;
	while(!isspace(ch)){
		if(ch == EOF) break;
		if(index > size - 2){
			size *= 2;
			string = reallocateString(string, size);
		}
		if(index <= 2)
			string[index++] = ch;
		ch = fgetc(fp);
	}

	ungetc(ch, fp);

	if (index > 0)              //there is something in the buffer
        	clearerr(fp);           //so force the read to be good

	string[index] = '\0';
	if(index == 0){
		printf("wtf\n");
		free(string);
		return NULL;
	}
	return string;
}

static char *reallocateString(char *string, size_t size){		
	char *newstring;

	newstring = realloc(string, size);

	return newstring;
}

