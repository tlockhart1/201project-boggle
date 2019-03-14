#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct STRING{
	char *string;
};

STRING *newSTRING(char *data){
	STRING *newstring = malloc(sizeof(STRING));
	assert(newstring != 0);
	newstring->string = data;
	return newstring;
}
char *getSTRING(STRING *data){
	return data->string;
}
char *setSTRING(STRING *data, char *newstr){
	char *old;
	old = data->string;
	data->string = newstr;
    return old;	
}
void displaySTRING(void *v, FILE *fp){
	fprintf(fp, "%s", getSTRING((STRING *) v));
}
void freeSTRING(void *data){
	STRING *s = (STRING*)data;
	if(s->string){
		//printf("AM I GETTING HERE\n");
		free(s->string);
	}
	free(s);
}
int compareSTRING(void *a, void *b){
	return strcmp(getSTRING(a), getSTRING(b));
}
